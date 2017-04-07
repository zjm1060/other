package com.bitc.jk.xmis;

import java.util.List;

import org.junit.Assert;
import org.junit.Test;

import com.bitc.jk.xmis.model.bid.BidCompany;
import com.bitc.jk.xmis.service.BidService;
import com.bitc.jk.xmis.test.SpringJunitBaseTest;

public class BidTest extends SpringJunitBaseTest {
	@Test
	public void testGetCompanies() {
		BidService bidService = (BidService) applicationContext.getBean("bidService");
		List<BidCompany> list = bidService.getBidCompanies();
		Assert.assertNotNull(list);
	}
}
