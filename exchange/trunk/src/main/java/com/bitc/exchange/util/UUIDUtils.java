package com.bitc.exchange.util;

public class UUIDUtils {
    public static String nextCode() {
        return java.util.UUID.randomUUID().toString()
                .toUpperCase().replaceAll("-", "");
    }

    public static void main(String[] args) {
        System.out.println(nextCode());
    }
}
