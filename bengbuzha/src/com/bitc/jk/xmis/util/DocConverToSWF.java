package com.bitc.jk.xmis.util;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * 格式转换
 */
public class DocConverToSWF {
	private static final int environment = 1;// 环境 1：windows 2:linux
	private String fileString;// (只涉及pdf2swf路径问题)
	private String outputPath = "";// 输入路径 ，如果不设置就输出在默认的位置
	private String fileName;
	private File pdfFile;
	private File swfFile;
	private File docFile;

	private String pageRead;// 读取第几页

	public DocConverToSWF(String fileString) {
		ini(fileString);
	}

	/**
	 * 重新设置file
	 * 
	 * @param fileString
	 */
	public void setFile(String fileString) {
		ini(fileString);
	}

	/**
	 * 初始化
	 * 
	 * @param fileString
	 */
	private void ini(String fileString) {
		this.fileString = fileString;
		fileName = fileString.substring(0, fileString.lastIndexOf("."));
		docFile = new File(fileString);
		pdfFile = new File(fileName + ".pdf");
		swfFile = new File(fileName + ".swf");
	}

	/**
	 * 多线程内部类 读取转换时cmd进程的标准输出流和错误输出流，这样做是因为如果不读取流，进程将死锁
	 * 
	 * @author iori
	 */
	private static class DoOutput extends Thread {
		public InputStream is;

		// 构造方法
		public DoOutput(InputStream is) {
			this.is = is;
		}

		public void run() {
			BufferedReader br = new BufferedReader(new InputStreamReader(
					this.is));
			String str = null;
			try {
				// 这里并没有对流的内容进行处理，只是读了一遍
				while ((str = br.readLine()) != null)
					;
			} catch (IOException e) {
				e.printStackTrace();
			} finally {
				if (br != null) {
					try {
						br.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
		}
	}

	/**
	 * 转换成 swf
	 */
	@SuppressWarnings("unused")
	private void pdf2swf() throws Exception {
		Runtime r = Runtime.getRuntime();
		if (!swfFile.exists()) {
			if (pdfFile.exists()) {
				if (environment == 1) {// windows环境处理
					try {
						// Process p =
						// r.exec("D:/Program Files/SWFTools/pdf2swf.exe "+
						// pdfFile.getPath() + " -o "+
						// swfFile.getPath().substring(0,swfFile.getPath().lastIndexOf("."))+"%.swf"
						// + " -T 9");
						Process p = r
								.exec("D:/Program Files/SWFTools/pdf2swf.exe "
										+ "\"" + pdfFile.getPath() + "\""
										+ " -o " + swfFile.getPath() + " -T 9"
										+ pageRead);

						// 非要读取一遍cmd的输出，要不不会flush生成文件（多线程）
						new DoOutput(p.getInputStream()).start();
						new DoOutput(p.getErrorStream()).start();
						try {
							// 调用waitFor方法，是为了阻塞当前进程，直到cmd执行完
							p.waitFor();
						} catch (InterruptedException e) {
							e.printStackTrace();
						}

						// System.out.print(loadStream(p.getInputStream()));
						// System.err.print(loadStream(p.getErrorStream()));
						// System.out.print(loadStream(p.getInputStream()));
						System.err.println("****文件来自：" + pdfFile.getPath()
								+ "****");
						System.err.println("****swf转换成功，文件输出："
								+ swfFile.getPath() + "****");
						if (pdfFile.exists()) {
							pdfFile.delete();
						}

					} catch (IOException e) {
						e.printStackTrace();
						throw e;
					}
				} else if (environment == 2) {// linux环境处理
					try {
						Process p = r.exec("pdf2swf " + pdfFile.getPath()
								+ " -o " + swfFile.getPath() + " -T 9");
						System.out.print(loadStream(p.getInputStream()));
						System.err.print(loadStream(p.getErrorStream()));
						System.err.println("****swf转换成功，文件输出："
								+ swfFile.getPath() + "****");
						if (pdfFile.exists()) {
							pdfFile.delete();
						}
					} catch (Exception e) {
						e.printStackTrace();
						throw e;
					}
				}
			} else {
				System.out.println("****pdf不存在,无法转换****");
			}
		} else {
			System.out.println("****swf已经存在不需要转换****");
		}
	}

	static String loadStream(InputStream in) throws IOException {

		int ptr = 0;
		in = new BufferedInputStream(in);
		StringBuffer buffer = new StringBuffer();

		while ((ptr = in.read()) != -1) {
			buffer.append((char) ptr);
		}

		return buffer.toString();
	}

	/**
	 * 转换主方法
	 */
	@SuppressWarnings("unused")
	public boolean conver() {

		if (swfFile.exists()) {
			System.out.println("****swf转换器开始工作，该文件已经转换为swf****");
			return true;
		}

		if (environment == 1) {
			System.out.println("****swf转换器开始工作，当前设置运行环境windows****");
		} else {
			System.out.println("****swf转换器开始工作，当前设置运行环境linux****");
		}
		try {
			pdf2swf();
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}

		if (swfFile.exists()) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * 返回文件路径
	 * 
	 * @param s
	 */
	public String getswfPath() {
		if (swfFile.exists()) {
			String tempString = swfFile.getPath();
			tempString = tempString.replaceAll("\\\\", "/");
			return tempString;
		} else {
			return "";
		}

	}

	/**
	 * 设置输出路径
	 */
	public void setOutputPath(String outputPath) {
		outputPath = outputPath.replace("\\", "/");
		fileName = fileName.replace("\\", "/");
		this.outputPath = outputPath;
		if (!outputPath.equals("")) {
			String realName = fileName.substring(fileName.lastIndexOf("/") + 1,
					fileName.length());
			if (outputPath.charAt(outputPath.length() - 1) == '/') {
				swfFile = new File(outputPath + realName + ".swf");
			} else {
				swfFile = new File(outputPath.substring(0,
						outputPath.lastIndexOf("/") + 1)
						+ realName + ".swf");
			}
		}
	}

	/**
	 * 读取第几页
	 * 
	 * @param pageRead
	 */
	public void setPage(String pageRead) {
		this.pageRead = pageRead;

	}

}