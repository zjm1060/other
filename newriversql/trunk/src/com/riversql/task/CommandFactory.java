package com.riversql.task;

import com.riversql.task.impl.MultiTask;

import javax.persistence.*;

/**
 * Created by zsz on 2015/8/18.
 */
@Entity
@Table(name="r_commands")
public class CommandFactory {
    @Id @GeneratedValue
    private int id;
    private String className;
    private String config;
    private byte[] configXML;
    private int sortNo;
    @ManyToOne(cascade = {CascadeType.MERGE,CascadeType.REFRESH})
    @JoinColumn(name = "task_id")
    private MultiTask multiTask;
    public CommandFactory() {
    }

    public CommandFactory(String className,String config, int sortNo) {
        this.className = className;
        this.config =config;
        this.sortNo = sortNo;
    }

    public Command buildCommand(){
        try {
            Class c=Class.forName(className);
            Command command=(Command) c.newInstance();
            command.setConfig(this.configXML);
            return command;
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        return null;
    }


    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getClassName() {
        return className;
    }

    public void setClassName(String className) {
        this.className = className;
    }

    public int getSortNo() {
        return sortNo;
    }

    public void setSortNo(int sortNo) {
        this.sortNo = sortNo;
    }

    public String getConfig() {
        return config;
    }

    public void setConfig(String config) {
        this.config = config;
    }
    public MultiTask getMultiTask() {
        return multiTask;
    }

    public void setMultiTask(MultiTask multiTask) {
        this.multiTask = multiTask;
    }

    @Lob
    @Basic(fetch = FetchType.EAGER)
    public byte[] getConfigXML() {
        return configXML;
    }

    public void setConfigXML(byte[] configXML) {
        this.configXML = configXML;
    }
}
