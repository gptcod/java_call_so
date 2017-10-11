//package com.fan.common;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;

public class NativeJava {

	private native int  generate_bloom_file(String white_list_file, String bloom_file);
	private native long  new_counting_bloom_from_file(int capacity, String bloom_file);
	private native int  counting_bloom_check(long bloom_pointer, String domain);
	private native String get_main_domain(String domain_name);
	private native int free_counting_bloom(long bloom_pointer);

	public static void main(String[] args) {
				int capacity = new NativeJava().generate_bloom_file("./all_white.txt", "./bloom.txt");
				System.out.println(capacity);
				int result;
				int num = 0;
				long bloom_pointer;

				bloom_pointer = new NativeJava().new_counting_bloom_from_file(capacity, "./bloom.txt");


				try {
					BufferedReader br = new BufferedReader(
							new InputStreamReader(new FileInputStream(new File("/home/liuyang/test/javatest/javatest_for_test/text.txt")), "UTF-8"));
					String lineTxt = null;
					while ((lineTxt = br.readLine()) != null) {
						
						// do anything
						// System.out.println(name + 1);
						result = new NativeJava().counting_bloom_check(bloom_pointer, lineTxt);

						if (result == 0) {
							// System.out.println(lineTxt);
							num +=1;
						}
					}
					br.close();
				} catch (Exception e) {
					System.err.println("read errors :" + e);
				}

				System.out.println(num);
				// new NativeJava().match_white_dns(capacity, "./bloom.txt", main_domain);

				//new NativeJava().match_white_dns(capacity, "./bloom.txt", "google.com");
				//System.out.println(System.getProperty("java.library.path")); 
	}
	
	static {
		System.load("/home/liuyang/test/javatest/javatest_for_test/libgoodluck.so");
	}
}
