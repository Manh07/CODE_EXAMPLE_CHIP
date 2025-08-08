;=================================
; do nhiet do
;=================================
start bit p1.6
eoc bit p1.4
ale bit p1.5

led1 bit p1.0
led2 bit p1.1
led3 bit p1.2
led4 bit p1.3
org 000h
main: lcall cdoi
lcall hex_bcd
lcall bcd_7doan
lcall hienthi
jmp main
cdoi:
setb ale
clr ale
setb start
jb eoc,$	   //??? chua hieu lam
clr start
mov r7,#150
de: lcall hienthi
djnz r7,de
mov a,p3
ret
hex_bcd:
mov b,#10
div ab
mov 10h,b			  
mov 11h,a			  
ret
bcd_7doan:
mov dptr,#900h
mov a,10h
movc a,@a + dptr
mov 20h,a				 // 20h chua hang chuc
mov a,11h
movc a,@a + dptr
mov 21h,a				// 21h chua hang donvi =>>
ret
hienthi: mov p0,21h
setb led4
lcall delay
anl p1,#0f0h	   ; p1=----1111

mov p0,20h
setb led3
lcall delay
anl p1,#0f0h

mov p0,#063h
setb led2
lcall delay
anl p1,#0f0h

mov p0,#039h
setb led1
lcall delay
anl p1,#0f0h

ret
delay: mov 7fh,#100
djnz 7fh,$
ret
org 900h
db 03fh,006h,05bh,04fh,066h,06dh,07dh,007h,07fh,06fh
END