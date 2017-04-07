package com.bitc.jk.xmis.export.web;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpServletResponseWrapper;

public class ResponseWrapper extends HttpServletResponseWrapper {

	private PrintWriter writer;

	public ResponseWrapper(HttpServletResponse response, PrintWriter writer) {
		super(response);
		this.writer = writer;
	}

	@Override
	public PrintWriter getWriter() throws IOException {
		return this.writer;
	}
}