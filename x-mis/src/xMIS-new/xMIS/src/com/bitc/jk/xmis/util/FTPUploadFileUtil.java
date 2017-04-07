package com.bitc.jk.xmis.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import sun.net.TelnetOutputStream;
import sun.net.ftp.FtpClient;

public class FTPUploadFileUtil {

	static FtpClient ftpClient= new FtpClient();
	
	static Lock lock= new ReentrantLock();

	public static void connectServer(String ip, String user, String password,
			String path) {

		try {
			lock.lock();
			ftpClient.openServer(ip);
			ftpClient.login(user, password);
			System.out.println("login success!");
			if (path.length() != 0)
				ftpClient.cd(path);
			ftpClient.binary();
		} catch (IOException e) {
			System.out.println("not login");
			e.printStackTrace();
			lock.unlock();
		}
	}

	public static void closeConnect() {
		try {
			if (ftpClient != null) {
				ftpClient.closeServer();
			}
			System.out.println("disconnect success");
		} catch (IOException e) {
			System.out.println("don't disconnect");
			e.printStackTrace();
		}finally{
			lock.unlock();
		}
	}

	public static boolean upload( File file, String ip,
			String user, String password, String path) {
			boolean success = false;
		
		try {
			connectServer(ip, user, password, path);
			TelnetOutputStream os = ftpClient.put(file.getName());
			InputStream is = new FileInputStream(file);
			byte[] bytes = new byte[1024];
			int c;
			while ((c = is.read(bytes)) != -1) {
				os.write(bytes, 0, c);
			}
			System.out.println("upload success");
			is.close();
			os.close();
			closeConnect();
			success = true;
		} catch (IOException e) {
			success = true;
			System.out.println("not upload");
			e.printStackTrace();
			lock.unlock();
		}
		return success;
	}
	
	public static void main(String[] args) {
		FTPUploadFileUtil.upload(new File("D:/F-1-20130329165933-139.txt"),"192.169.11.27","sms","xmis","/");
	}

}
