
MakeConfig(){

Temp_=$1
Mat_=$2
Prefix_=$3
cat > $Prefix_.cfg <<EOF

ReferenceClass_{  NOT_NULL URS_Curve { 
    Variables {  
        MatterMol ReferenceClass_{ NOT_NULL       EOS_Savable { MatterFreeE FreeERossH2     Material_File mat.bad Substance H2Mol } }
        MatterMet ReferenceClass_{ NOT_NULL       EOS_Savable { MatterFreeE FreeERossH2     Material_File mat.bad Substance H2Met } }
        MatterSumMet ReferenceClass_{ NOT_NULL    EOS_Savable { MatterFreeE FreeESumMatter  Material_File mat.bad Substance MatterSumH2Met } }

        Reader  ReferenceClass_{  NOT_NULL UrsCurve_FileReader { FileName trCold.dat DataNames {  Denc Pres 3 4  } DefaultValue  0 }   }   
        Temperature   ReferenceClass_{  NOT_NULL UrsCurve_StepClc { MinVal 20 MaxVal 1000  NumDivStp 1  LogScale 1 NumSame 1  Centered 0 }    }  
        TemperatureConst   ReferenceClass_{  NOT_NULL UrsCurve_StepClc { MinVal $Temp_ MaxVal $Temp_  NumDivStp 1  LogScale 0 NumSame 1  Centered 0 }    }  

	BndMat  ReferenceClass_{  NOT_NULL UrsCurve_PT_Bnd_Constructor { NameTemp Temperature NameMatterHghP MatterMet NameMatterLowP MatterMol ClcError 1e-006 FindPDencFrom 0.1 FindPDencTo 2 FindGLowPres 0.1 FindGHghPres 300 StartPres 100 }   }   

        IsoTMol      ReferenceClass_{  NOT_NULL UrsCurve_FreeE { NameDenc Reader.Denc NameTemp TemperatureConst NameMatter MatterMol }   }   
        IsoTMet      ReferenceClass_{  NOT_NULL UrsCurve_FreeE { NameDenc Reader.Denc NameTemp TemperatureConst NameMatter MatterMet }   }   
        IsoTSum      ReferenceClass_{  NOT_NULL UrsCurve_FreeE { NameDenc Reader.Denc NameTemp TemperatureConst NameMatter MatterSumMet }   }   
    }

    Output   ReferenceClass_{  NOT_NULL URS_Curve::Output { 
        FileToStore $Prefix_.dat VectorOfNames { 
             $Mat_.Pressure $Mat_.FreeG 
        }
    } }  
NumIter  25   } }

EOF
}
Compare_mat(){
	MakeConfig $1 $2 P_G.$1.$2
	MakeConfig $1 $3 P_G.$1.$3	
	urs_curve P_G.$1.$2.cfg
	urs_curve P_G.$1.$3.cfg
	set1grph P_G.$1.$2.dat P_G.$1.$3.dat P_G.$1.$2_$3.dat /i /a
	
}

Usage(){
    echo 'usage: '$0' -c 1400 Mat1 Mat2'
    echo '              generate G(P) sum for Mat1 Mat2'
    echo '              Mat= IsoTMet IsoTMol IsoTSum '

}


while getopts "c" opt; do
    case $opt in
    c  ) COMPARE_MAT=1;;
    \? )  Usage
         exit 1
    esac
done

if   [ ! -z $COMPARE_MAT ]
then
    Compare_mat $2 $3 $4
    exit 1
fi

Usage