package bitc.sz.wzEnv.security.model;

import org.hibernate.annotations.GenericGenerator;

import javax.persistence.*;
import java.io.Serializable;

@Entity
@Table(name = "t_gjs_role")
public class Role implements Serializable {
    private static final long serialVersionUID = -3344113622027926777L;

    @Id
    @GenericGenerator(name = "idGenerator", strategy = "org.hibernate.id.Assigned")
    @GeneratedValue(generator = "idGenerator")
    private String c_id;

    @Column
    private String c_rolenm;

    @Column
    private Integer n_type;

    @Column
    private String c_info;

    public String getC_id() {
        return c_id;
    }

    public void setC_id(String c_id) {
        this.c_id = c_id;
    }

    public String getC_info() {
        return c_info;
    }

    public void setC_info(String c_info) {
        this.c_info = c_info;
    }

    public String getC_rolenm() {
        return c_rolenm;
    }

    public void setC_rolenm(String c_rolenm) {
        this.c_rolenm = c_rolenm;
    }

    public Integer getN_type() {
        return n_type;
    }

    public void setN_type(Integer n_type) {
        this.n_type = n_type;
    }
}
