package test;

import org.junit.Test;
import org.springframework.beans.factory.annotation.Autowired;

import com.bitc.jk.xmis.service.DeviceService;
import com.bitc.jk.xmis.test.SpringJunitBaseTest;

public class DeviceActionTest extends SpringJunitBaseTest {

	@Autowired
	private DeviceService deviceService;

	@Test
	public void testAddDeviceAddress() {
		String fid = "C4682A70BF0F4ACDB6C5038F4F715B74";
		String type = "position";
		String name = "junitname";
		String code = "junitcode";

		deviceService.doAddDeviceAddress(type, fid, name, code);
		System.out.println("success");
	}


}
