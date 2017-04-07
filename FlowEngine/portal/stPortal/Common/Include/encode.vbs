Function URLEncodComponent(vstrIn)
  strReturn = ""
  For t = 1 To Len(vstrIn)
  ThisChr = Mid(vStrIn,t,1)
  If Abs(Asc(ThisChr)) < &HFF Then
  strReturn = strReturn & ThisChr
  Else
  innerCode = Asc(ThisChr)
  If innerCode < 0 Then
  innerCode = innerCode + &H10000
  End If
  Hight8 = (innerCode And &HFF00)\ &HFF
  Low8 = innerCode And &HFF
  strReturn = strReturn & "%" & Hex(Hight8) & "%" & Hex(Low8)
  End If
  Next
  URLEncodComponent = strReturn
  End Function
  