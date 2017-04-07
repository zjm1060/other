<%--public static final String replace( String line, String oldString, String newString ) {
if (line == null) {
return null;
}
int i=0;
if ( ( i=line.indexOf( oldString, i ) ) >= 0 ) {
char [] line2 = line.toCharArray();
char [] newString2 = newString.toCharArray();
int oLength = oldString.length();
StringBuffer buf = new StringBuffer(line2.length);
buf.append(line2, 0, i).append(newString2);
i += oLength;
int j = i;
while(( i=line.indexOf(oldString, i)) > 0) {
buf.append(line2, j, i-j).append(newString2);
i += oLength;
j = i;
}
buf.append(line2, j, line2.length - j);
return buf.toString();
}
return line;
}

public String simplereplace(String line,String oldStr,String newStr)
{
int index = 0;
while ((index = line.indexOf(oldStr, index)) >= 0)
{
line = line.substring(0, index) + newStr + line.substring(index + oldStr.length());
index += newStr.length();
}
return line;
}

/*use Calendar object to compare
java.util.Calendar class can be used to compare date. In order to do this, you guy should parse that string into int year, month, day and construct a Calendar object, and then do comparison.

Below is a sample
*/
StringTokenizer token = new StringTokenizer(your string,"-");
int year = Integer.parseInt(token.nextToken());
int month = Integer.parseInt(token.nextToken());
int day = Integer.parseInt(token.nextToken());
Calendar date = Calendar.getInstance();
date.set(year,month,day);
Calendar today = Calendar.getInstacne();
if(date.after(today)){
//......
}


public final class Replace{
public static java.lang.String replace(java.lang.String strSource,java.lang.String strFrom,java.lang.String strTo){
java.lang.String strDest = "";
int intFromLen = strFrom.length();
int intPos;

while((intPos=strSource.indexOf(strFrom))!=-1){
strDest = strDest + strSource.substring(0,intPos);
strDest = strDest + strTo;
strSource = strSource.substring(intPos+intFromLen);
}
strDest = strDest + strSource;

return strDest;
}
}
--%>
<%!
          public String GetValue(String aValue)
          {
                    String sValue = null;
                    if (aValue==null)
                              sValue = "";
                    else
                              sValue = aValue;

                    return sValue;
          }

	public String GetValueN(String aValue)
          {
		String sValueN=null;
                    if (aValue==null)
			sValueN = "0";
		else
			sValueN = aValue;

                    return sValueN;
	}


%>