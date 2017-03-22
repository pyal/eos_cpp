#pragma once

#include "lib/std/ex_out.h"
#include "lib/Ref/file_manip.h"
#include <deque>

class QtMenuElement: public QObject {
public:
    virtual list<QWidget*> BuildWidget() = 0;
    virtual void Connect(const QObject *receiver, const char *member) = 0;
};

class QtData {
public:
    using p2 = pair<double, double>;
    struct Data {
        string name;
        vector <p2> pnts;
    };
    static vector<Data> BuildData(const vector<vector<double>> &pnts, const vector<string> &names) {
        vector<Data> ret;
        for(size_t i = 1; i < names.size(); i++) ret.push_back({names[i], {}});
        for(auto &line: pnts) {
            verify(line.size() == names.size(), "Lines have different column number");
            for(size_t i = 1; i < line.size(); i++) {
                if(line[0] != NullNumber && line[i] != NullNumber)
                    ret[i - 1].pnts.push_back({line[0], line[i]});
            }
        }
        return ret;
    }
    class IDataSource: public RefCount {
    public:
        virtual pair<string, vector<Data>> &getData() = 0;
        virtual bool getNext() = 0;
        virtual ~IDataSource(){};
    };
};

class IFileData: public QtData::IDataSource {
    pair<string, vector<QtData::Data>> CurData;
    string NewDataName;
public:
    IFileData():OneLineToMany(false) {}
    bool OneLineToMany;
    virtual pair<vector<string>, vector<double>> ReadHead() = 0;
    virtual bool isEnd(vector<double> &row_pnt) = 0;
    virtual string PlotName() = 0;
    virtual pair<string, vector<QtData::Data>> &getData() { return CurData;};
    virtual bool getNext() {
        log_debug(Stroka("Reading next data OneLineToMany ") + OneLineToMany);
        auto head = ReadHead();
        vector<vector<vector<double>>> points_arr(1);
        if(head.first.size() == 0) return false;
        if(head.second.size() > 0) points_arr.back().push_back(head.second);
        vector<double> row;
        int rowNum = 0;
        double prevX = -1./M_MinDouble;
        while(!isEnd(row)) {
            if (row.size() == head.first.size()) {
                if(OneLineToMany && row.front() < prevX) {
                    points_arr.push_back(vector<vector<double>>());
                }
                points_arr.back().push_back(row);
                prevX = row.front();
            }
            else {
                log_always(Stroka("Skipping row num: ") + rowNum + " content: " + Str::Vec2Str(row));
            }
            rowNum++;
        }
        CurData.second.clear();
        CurData.first = PlotName();
        int ar_num = 0;
        for(auto &points: points_arr) {
            auto names = head.first;
            if(points_arr.size() > 1) {
                for(auto &n:names) n = string(n, 0, 1) + "_" + to_string(ar_num);
            }
            auto pnts = QtData::BuildData(points, names);
            for(auto &pnt: pnts) CurData.second.push_back(pnt);
            ar_num++;
        }
        return true;
    }
};

class SingleFile: public IFileData {
    string FileName;
    FILE *In;
    char FieldDelim;
public:
    SingleFile(const string &file, char fieldDelim, bool oneLineToMany):FileName(file),
       In(File::open(file.c_str(), "r", "SingleFile")), FieldDelim(fieldDelim) {
            OneLineToMany = oneLineToMany;
        log_debug(Stroka("OneLineToMany ") + OneLineToMany);
    }
    ~SingleFile() {
        File::close(In);
    }
    virtual string PlotName() {
        return FileName;
    }
    virtual pair<vector<string>, vector<double>> ReadHead() {
        pair<vector<string>, vector<double>> prev, cur;
        while(true) {
            cur = ReadHead_int();
            if(cur.first.size() == 0) return cur;
            if(cur.second.size() != 0) {
                if(prev.first.size() == cur.second.size()) {
                    cur.first = prev.first;
                    return cur;
                }
                return cur;
            }
            prev = cur;
        };
        return cur;
    };
    virtual bool isEnd(vector<double> &row_pnt) {
        vector<Stroka> tmp;
        row_pnt.clear();
        double d;
        while(row_pnt.size() == 0) {
            if (!File::GetLine(In, tmp, FieldDelim)) return true;
//            log_always("Read line " + Str::Vec2Str(tmp));
            for(auto &str: tmp) {
                if (IsDouble(str.c_str(), d, 1)) row_pnt.push_back(d);
                else {
                    row_pnt.clear();
                    break;
                }
            }
        }
        return false;

    }
private:
    pair<vector<string>, vector<double>> ReadHead_int() {
        double d;
        pair<vector<string>, vector<double>> ret;
        do {
            if (!File::GetLine(In, ret.first, FieldDelim)) return ret;
        } while(ret.first.size() == 0);
        for(auto &str: ret.first) if(IsDouble(str.c_str(), d, 1)) ret.second.push_back(d);
        if(ret.second.size() == ret.first.size()) {
            for(int i = 0; i < ret.first.size(); i++)
                ret.first[i] = ~(Stroka("c_") + i);
        } else {
            ret.second.clear();
        }
        return ret;
    };
};


class MultiFile: public QtData::IDataSource {
    vector<Stroka> FileName;
    vector<Ref<SingleFile>> FileReaders;
    pair<string, vector<QtData::Data>> CurData;
public:
    MultiFile(const string &files, char fieldDelim, bool oneLineToMany): FileName(Str::SplitLine(Stroka(files.c_str()))) {
        for(auto &name: FileName)
            FileReaders.push_back(new SingleFile(name, fieldDelim, oneLineToMany));
    }
    ~MultiFile(){}
    virtual pair<string, vector<QtData::Data>> &getData() {
        return CurData;
    };
    virtual bool getNext() {
        bool notAllDone = false;
        for(auto &reader: FileReaders)
            notAllDone |= reader->getNext();
        if (notAllDone) {
            CurData.second.clear();
            CurData.first = ~Str::JoinLine(FileName, ":");
            for(auto &reader: FileReaders) {
                auto &add = reader->getData();
                CurData.second.insert(CurData.second.end(), add.second.begin(), add.second.end());
            }
        }
        return notAllDone;
    }

};



class GasdData: public IFileData {
    string FileName, NewTimeName;
    string PlotNameStr;
    FILE *In;
    vector<string> ColumnNames;
    bool CurTime2ColNames;
public:
    ~GasdData() {
        File::close(In);
    }
    GasdData(const string &file, const string &newTimeName, bool curTime2ColNames):
            FileName(file), NewTimeName(newTimeName), In(File::open(file.c_str(), "r", "SingleFile")),
            CurTime2ColNames(curTime2ColNames){
    }
    virtual string PlotName() {
        log_debug("PlotName: " + PlotNameStr);
        return PlotNameStr;
    }
    virtual pair<vector<string>, vector<double>> ReadHead() {
        log_debug("Reading head ");
        pair<vector<string>, vector<double>> ret;
        if(ColumnNames.size() != 0) {
            ret.first = ColumnNames;
            return ret;
        }
        vector<string> buf;
        deque<vector<string>> que;
        for(auto i:{1,2}) {
            unused(i);
            buf.clear();
            while(buf.size() == 0) if (!File::GetLine(In, buf)) return ret;
            que.push_back(buf);
        }
        while(que.back()[0] != NewTimeName) {
            buf.clear();
            while(buf.size() == 0) if (!File::GetLine(In, buf)) return ret;
            que.push_back(buf);
            que.pop_front();
        }
        assert(que.back().size() > 1);
        PlotNameStr = que.back()[1];
        if(!CurTime2ColNames) {
            ret.first = que.front();
        } else {
            buf.clear();
            while(buf.size() == 0) if (!File::GetLine(In, buf)) return ret;
            ret.first = buf;
        }
        ColumnNames = ret.first;
        return ret;
    };
    virtual bool isEnd(vector<double> &row_pnt) {
//        log_debug("Reading points");
        vector<string> tmp;
        row_pnt.clear();
        double d;
        while(row_pnt.size() == 0) {
            tmp.clear();
            while(tmp.size() == 0) if (!File::GetLine(In, tmp)) return true;
            if(tmp[0] == NewTimeName) {
                assert(tmp.size() > 1);
                PlotNameStr = tmp[1];
                return true;
            }
            for(auto &str: tmp) {
                if (IsDouble(str.c_str(), d, 1)) row_pnt.push_back(d);
                else {
                    row_pnt.clear();
                    break;
                }
            }
        }
        return false;

    }


};

