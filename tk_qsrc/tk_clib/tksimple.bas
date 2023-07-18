print "Test"
input "Input Test> "; t0
print "Got "; t0

if t0>=0 and t0<=4 then goto lbl0to4
if t0>=5 and t0<=9 then goto lbl5to9

goto lbl_other

lbl0to4:
print "Value was between 0 and 4"
goto done

lbl5to9:
print "Value was between 5 and 9"
goto done

lbl_other:
print "Some other value was given"

done:
