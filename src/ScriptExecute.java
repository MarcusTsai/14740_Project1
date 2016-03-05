import java.io.*;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

/**
 * Created by mingchia on 3/5/16.
 */
public class ScriptExecute {

    public static File execute (HashMap<String, String> args, File file) throws Exception{

        // Iterate args to envargs
        String[] envargs = new String[args.size()];

        Set<String> keys = args.keySet();
        int i = 0;
        for(String key: keys) {
            envargs[i] = key + "=" + args.get(key);
            i++;
        }

        String[] cmd = {"perl", file.toString()};
        Process process = Runtime.getRuntime().exec(cmd, envargs, file.getParentFile());


        File output = new File("output.html");
        FileOutputStream out = new FileOutputStream(output);

        InputStream input = process.getInputStream();

        byte[] buffer = new byte[4096];
        int bytesRead =input.read(buffer, 0 , 4096);
//        while ((bytesRead = input.read(buffer, 0, 4096)) != -1) {
//            out.write(buffer, 0, bytesRead);
//            System.out.print(bytesRead);
//            System.out.print("HI");
//        }
        System.out.println("hahah  "+ buffer);
        out.close();
        input.close();


        return output;
    }

}
