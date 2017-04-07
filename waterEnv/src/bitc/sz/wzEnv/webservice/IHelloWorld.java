package bitc.sz.wzEnv.webservice;
import javax.jws.WebParam;
import javax.jws.WebService;

/**
 * Created by BomWu on 2015/12/9 0009.
 */

@WebService
public interface IHelloWorld {
    String sayHelloWorldFrom(@WebParam(name="from") String from);
}
