C     This File is Automatically generated by ALOHA 
C     The process calculated in this file is: 
C     Metric(1,4)*Metric(2,3) - Metric(1,2)*Metric(3,4)
C     
      SUBROUTINE VVVV3P1N_3(V1, V2, V4, COUP,V3)
      IMPLICIT NONE
      COMPLEX*16 CI
      PARAMETER (CI=(0D0,1D0))
      COMPLEX*16 COUP
      COMPLEX*16 TMP1
      COMPLEX*16 TMP10
      COMPLEX*16 V1(*)
      COMPLEX*16 V2(*)
      COMPLEX*16 V3(6)
      COMPLEX*16 V4(*)
      TMP1 = (V2(3)*V1(3)-V2(4)*V1(4)-V2(5)*V1(5)-V2(6)*V1(6))
      TMP10 = (V1(3)*V4(3)-V1(4)*V4(4)-V1(5)*V4(5)-V1(6)*V4(6))
      V3(3)= COUP*(-CI*(V2(3)*TMP10)+CI*(TMP1*V4(3)))
      V3(4)= COUP*(+CI*(V2(4)*TMP10)-CI*(TMP1*V4(4)))
      V3(5)= COUP*(+CI*(V2(5)*TMP10)-CI*(TMP1*V4(5)))
      V3(6)= COUP*(+CI*(V2(6)*TMP10)-CI*(TMP1*V4(6)))
      END

