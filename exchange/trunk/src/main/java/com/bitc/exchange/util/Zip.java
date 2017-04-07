package com.bitc.exchange.util;

import org.apache.commons.compress.archivers.zip.Zip64Mode;
import org.apache.commons.compress.archivers.zip.ZipArchiveEntry;
import org.apache.commons.compress.archivers.zip.ZipArchiveOutputStream;
import org.apache.commons.compress.archivers.zip.ZipFile;
import org.apache.commons.compress.utils.IOUtils;
import org.apache.commons.io.FileUtils;

import java.io.*;
import java.util.Enumeration;

public class Zip {

    /**
     * 解压zip文件到当前目录
     *
     * @param zip
     * @throws java.io.IOException
     */
    public static void unZip(File zip) throws IOException {
        unZip(zip, zip.getParent());
    }

    /**
     * 解压zip文件
     *
     * @param zipFile   需要解压的文件
     * @param outputDir 将文件解压到目录
     * @throws java.io.IOException
     */
    public static void unZip(File zipFile, String outputDir) throws IOException {
        if (zipFile.exists()) {
            outputDir = outputDir + File.separator;
            FileUtils.forceMkdir(new File(outputDir));

            ZipFile zf = new ZipFile(zipFile);
            Enumeration<ZipArchiveEntry> zipArchiveEntrys = zf.getEntries();
            while (zipArchiveEntrys.hasMoreElements()) {
                ZipArchiveEntry zipArchiveEntry = zipArchiveEntrys.nextElement();
                if (zipArchiveEntry.isDirectory()) {
                    FileUtils.forceMkdir(new File(outputDir + zipArchiveEntry.getName() + File.separator));
                } else {
                    FileOutputStream fos = FileUtils.openOutputStream(new File(outputDir + zipArchiveEntry.getName()));
                    IOUtils.copy(zf.getInputStream(zipArchiveEntry), fos);
                    IOUtils.closeQuietly(fos);
                }
            }
            zf.close();
        } else {
            throw new IOException("zip file not found:" + zipFile.getPath());
        }
    }

    public static void zip(File[] files, OutputStream zipOs) {

        if (files == null || files.length == 0) {
            return;
        }
        ZipArchiveOutputStream zaos = null;
        try {
            zaos = new ZipArchiveOutputStream(zipOs);
            zaos.setUseZip64(Zip64Mode.AsNeeded);
            zipFiles(files, zaos, null);
            zaos.finish();
        } catch (Exception e) {
            throw new RuntimeException(e);
        } finally {
            try {
                if (zaos != null) {
                    zaos.close();
                }
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    private static void zipFiles(File[] files, ZipArchiveOutputStream zaos, String pathName) throws IOException {
        for (File file : files) {
            if (file == null || !file.exists()) {
                continue;
            }
            if (file.isDirectory()) {
                zipFiles(file.listFiles(), zaos, file.getName());
            } else {
                ZipArchiveEntry zipArchiveEntry = new ZipArchiveEntry(file,
                        pathName == null ? file.getName() : pathName + File.separator + file.getName());
                zaos.putArchiveEntry(zipArchiveEntry);
                InputStream is = new FileInputStream(file);
                IOUtils.copy(is, zaos);
                IOUtils.closeQuietly(is);
                zaos.closeArchiveEntry();
            }
        }
    }
}
