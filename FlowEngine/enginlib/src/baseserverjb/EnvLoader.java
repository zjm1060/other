package baseserverjb;

import java.io.FileInputStream;
import java.util.Random;

final class EnvLoader
{
    private static String _$264;

    public static final String getEnv(String path)
    {
        FileInputStream stream = null;
        String xmlFile = "";
        _$264 = path + "EnviromentVarible.xml";

        try
        {
            stream = new FileInputStream(_$264);
            byte[] buf = new byte[stream.available()];
            stream.read(buf, 0, buf.length);
            xmlFile = new String(buf);
            Random ran = new Random();
            long lTag = ran.nextLong();
            double d = Math.random();
            int iCount = (int)(d * 10.0D);
            StringBuffer sb = new StringBuffer("");
            sb.append("<" + lTag + "," + iCount + ">");
            for (int i = 0; i < iCount; i++) {
                sb.append(";" + lTag);
            }
            sb.append(xmlFile);
            xmlFile = sb.toString();
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }
        return xmlFile;
    }

    public static final boolean checkUser(int userCount)
    {
        return true;
    }

    public static final boolean checkFlow(int flowCount)
    {
        return true;
    }

    public static void main(String[] args)
    {
        try
        {
            System.out.println(getEnv(""));
        }
        catch (Exception ex)
        {
            ex.printStackTrace();
        }
    }
}
