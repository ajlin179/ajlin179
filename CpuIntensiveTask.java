package com.alwin.hello;

import java.math.BigInteger;


// cat /proc/5024/stat | awk '{ print $14; print $15; print $16; print $17;print $20}' ; cat /proc/5036/stat | awk '{ print $14; print $15; print $16; print $17;print $20}' ;  cat /proc/5037/stat |  awk '{ print $14; print $15; print $16; print $17;print $20}'
public class CpuUtilizationApp {


    public static void main(String[] args) throws Exception {
        Thread t1 =   new Thread(new CpuIntensiveTask("T1", 1000000));
        Thread t2 = new Thread(new CpuIntensiveTask("T2", 3000000));
        t1.start();
        t2.start();
        t1.join();
        t2.join();

        Thread.sleep(1000 * 60 * 1);
    }


    static class CpuIntensiveTask implements Runnable {
        private String name;
        private int loopCount;

        CpuIntensiveTask(String lName, int lLoopCount) {
            name = lName;
            loopCount = lLoopCount;
        }

        @Override
        public void run() {

            while (loopCount-- > 0) {
                int num = 30;
                BigInteger factorial = BigInteger.ONE;
                for (int i = 1; i <= num; ++i) {
                    // factorial = factorial * i;
                    factorial = factorial.multiply(BigInteger.valueOf(i));
                }
                System.out.printf("%s Factorial of %d = %d", name, num, factorial);
            }
        }
    }


}
