import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;

public class TXTTransUtils {
	public static void main(String[] args) {

		/* 读取数据 */
		try {
			BufferedReader br = new BufferedReader(
					new InputStreamReader(new FileInputStream(new File("/home/text.txt")), "UTF-8"));
			String lineTxt = null;
			while ((lineTxt = br.readLine()) != null) {
				
				// do anything
				System.out.println(name + 1);
				
			}
			br.close();
		} catch (Exception e) {
			System.err.println("read errors :" + e);
		}

	}
}