package com.riversql.task;


/**
 * Created by zsz on 2015/8/17.
 */
public interface Command {
    void execute();

    void rollback();

    void setConfig(byte[] config);
}
