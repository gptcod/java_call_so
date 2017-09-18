//package com.fan.common;

public class NativeJava {
	private native int  match_white_dns(String white_dns);
	public static void main(String[] args)
			{
				new NativeJava().match_white_dns("baidu.com");
				//System.out.println(System.getProperty("java.library.path")); 
			}
	
	static {
		System.load("/home/liuyang/test/javatest/libgoodluck.so");
	}
}
