package com.riversql.task;

import com.riversql.quartz.SchedulerManage;
import com.riversql.task.impl.MultiTask;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import javax.persistence.Query;
import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;
import java.util.List;

/**
 * Created by zsz on 2015/8/17.
 */
public class TaskManager implements ServletContextListener {
    private List<Task> taskList;

    public void startTask(Task task) {

    }

    @Override
    public void contextInitialized(ServletContextEvent sce) {
        try {
            System.out.println("TaskManager contextInitialized**********************************");
            ServletContext sc=sce.getServletContext();
            EntityManagerFactory emf= (EntityManagerFactory) sc.getAttribute("emf");
            EntityManager em = emf.createEntityManager();
            SchedulerManage.start();
            Query query = em.createQuery("select o from MultiTask o where o.state = 1");
            List<MultiTask> list =  query.getResultList();
            for(MultiTask task : list){
                    SchedulerManage.addTask(task);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    @Override
    public void contextDestroyed(ServletContextEvent servletContextEvent) {

    }
}
