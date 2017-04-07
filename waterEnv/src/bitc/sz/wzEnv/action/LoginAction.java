package bitc.sz.wzEnv.action;

import bitc.sz.wzEnv.security.model.UserImpl;
import bitc.sz.wzEnv.util.ExtjsResponse;
import org.apache.shiro.SecurityUtils;
import org.apache.shiro.authc.*;
import org.apache.shiro.subject.Subject;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

/**
 * Created by zsz on 2016/2/19.
 */
@Controller
public class LoginAction {

    @ResponseBody
    @RequestMapping("/login")
    public ExtjsResponse login(UserImpl user){
        Subject currentUser = SecurityUtils.getSubject();
        UsernamePasswordToken token = new UsernamePasswordToken(
                user.getName(),user.getPassword());
        try {
            currentUser.login(token);
        } catch (UnknownAccountException uae) {
            return new ExtjsResponse(false,"登录失败","用户名或密码错误，登录失败。");
        } catch (IncorrectCredentialsException ice) {
            return new ExtjsResponse(false ,"登录失败","用户名或密码错误，登录失败。");
        } catch (LockedAccountException lae) {
            return new ExtjsResponse(false,"登录失败", "账户被锁定，请与管理员联系。");
        } catch (ExcessiveAttemptsException eae) {
            return new ExtjsResponse(false,"登录失败","尝试次数超过限制，账户被锁定，请联系管理员。");
        }
        return new ExtjsResponse(true);
    }

    @RequestMapping("/logout")
    public String logout() {
        Subject currentUser = SecurityUtils.getSubject();
        currentUser.logout();
        return "redirect:/";
    }

}
