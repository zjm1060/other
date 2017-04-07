package com.riversql.task.impl;

import com.riversql.task.Command;
import com.riversql.task.CommandFactory;
import com.riversql.task.Task;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by zsz on 2015/8/17.
 */
@Entity
@Table(name = "r_multi_task")
public class MultiTask implements Task {
    @Id
    @GeneratedValue
    private int id;
    private String name;
    @OneToMany(cascade = CascadeType.ALL,fetch = FetchType.EAGER,mappedBy = "multiTask")
    private List<CommandFactory> commandList = new ArrayList<CommandFactory>();
    private String cronTrigger;
    private  int state = 0;
    public MultiTask(){

    }
    public void setCronTrigger(String cronTrigger) {
        this.cronTrigger = cronTrigger;
    }

    @Override
    public String getCronTrigger() {
        return this.cronTrigger;

    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    @Override
    public String getName() {
        return this.name;
    }

    public void setName(String name) {
        this.name = name;
    }
    public List<CommandFactory> getCommandList() {
        return commandList;
    }

    public void setCommandList(List<CommandFactory> commandList) {
        this.commandList = commandList;
    }

    @Column(nullable = false)
    public int getState() {
        return state;
    }

    public void setState(int state) {
        this.state = state;
    }

    @Override
    public void execute() {
        for (CommandFactory commandFactory : getCommandList()) {
            Command command = commandFactory.buildCommand();
            if (command != null) {
                command.execute();
            }
        }
    }
    public  void addCommand(CommandFactory commandFactory){
        commandFactory.setMultiTask(this);
        this.commandList.add(commandFactory);
    }
}
