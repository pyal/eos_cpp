C* This program generates an array ph(2048) of random magnetic fields.
C* r(250) is used in the random numbers generator
C* JT is the physical ratio J/T

Subroutine MC_Step(ph,JT,r)
Real*8 ph(2048),JT,phk(-2048:2048),ps,w,Pi
Real*8 c(2048,1024),s(2048,1024)
Integer i,j,len
Integer*4 r(250)
len=2048
Pi=datan(1.d0)*4.d0
Do i=1,len
Do j=1,len/2
c(i,j)=dcos((i-5.d-1)*Pi/len*(j-5.d-1))
s(i,j)=dsin((i-5.d-1)*Pi/len*(j-5.d-1))
Enddo
Enddo
w=1.d0/2.d0**30
Do i=1,len/2
!-------Generates random number ps according to the gaussian distribution
ps=0.d0
Do j=1,2
Do k=1,103
r(k)=ieor(r(k),r(k+147))
ps=ps+r(k)*w-1.d0
Enddo
Do k=104,250
r(k)=ieor(r(k),r(k-103))
ps=ps+r(k)*w-1.d0
Enddo
Enddo
ps=ps/dsqrt(5.d2)
!-------------End
phk(i)=ps*dsqrt(len*3.d0/(4.d0*Pi*(i-5.d-1)*JT)) !----random Fourier components
!-------Generates random number ps according to the gaussian distribution
ps=0.d0
Do j=1,2
Do k=1,103
r(k)=ieor(r(k),r(k+147))
ps=ps+r(k)*w-1.d0
Enddo
Do k=104,250
r(k)=ieor(r(k),r(k-103))
ps=ps+r(k)*w-1.d0
Enddo
Enddo
ps=ps/dsqrt(5.d2)
!------------End
phk(-i)=ps*dsqrt(len*3.d0/(4.d0*Pi*(i-5.d-1)*JT)) !----random Fourier components 
Enddo
Do i=1,len
ph(i)=0.d0
Do j=1,len/2
ph(i)=ph(i)+phk(j)*c(i,j)+phk(-j)*s(i,j)
Enddo
ph(i)=ph(i)*dsqrt(2.d0/len)
Enddo
Return
End


