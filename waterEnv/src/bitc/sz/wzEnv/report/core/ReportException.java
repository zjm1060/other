package bitc.sz.wzEnv.report.core;

public class ReportException extends Exception {

    public ReportException() {
    }

    public ReportException(String message) {
        super(message);
    }

    public ReportException(String message, Throwable cause) {
        super(message, cause);
    }

    public ReportException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
        super(message, cause, enableSuppression, writableStackTrace);
    }

    public ReportException(Throwable cause) {
        super(cause);
    }

    private static final long serialVersionUID = -8555381638399915517L;

}
