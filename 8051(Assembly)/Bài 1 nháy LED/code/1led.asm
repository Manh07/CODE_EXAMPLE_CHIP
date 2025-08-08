	led bit P2.0
org 00h

main:
      clr led
      lcall delay_500ms
      setb led
      lcall delay_500ms
      sjmp main
delay_500ms :
      MOV 50H,#200
loop: MOV 51H,#250
      DJNZ 51H,$
      DJNZ 50H,loop
   RET
end