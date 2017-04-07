package com.riversql.task;

import org.quartz.*;

/**
 * Created by zsz on 2015/8/17.
 */
public interface Task {
    String getCronTrigger();

    String getName();

    void execute();
}
