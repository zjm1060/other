package com.bitc.jk.xmis.test;

import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionDefinition;
import org.springframework.transaction.TransactionException;
import org.springframework.transaction.TransactionStatus;

public class MockedTransactionManager implements PlatformTransactionManager {

	public TransactionStatus getTransaction(TransactionDefinition definition)
			throws TransactionException {
		return null;
	}

	public void commit(TransactionStatus status) throws TransactionException {

	}

	public void rollback(TransactionStatus status) throws TransactionException {

	}
}