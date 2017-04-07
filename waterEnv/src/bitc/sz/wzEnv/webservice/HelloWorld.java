package bitc.sz.wzEnv.webservice;
import javax.jws.WebParam;
import javax.jws.WebService;

/**
 * Created by BomWu on 2015/12/9 0009.
 */

@WebService(endpointInterface = "bitc.sz.wzEnv.webservice.IHelloWorld", serviceName = "IHelloWorld")
public class HelloWorld implements IHelloWorld {
      public String sayHelloWorldFrom(String from) {
           String result = "Hello, world, from " + from;
           System.out.println(result);
           return result;
      }
}
