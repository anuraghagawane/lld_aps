class LogMessage {
    level: number;
    message: string;
    constructor(level: number, message: string) {
        this.level = level;
        this.message = message;
    }
}

interface Logger {
    setNext(next: Logger): void;
    log(msg: LogMessage): void;
}

abstract class BaseLogger implements Logger {
    protected next: Logger | null = null;
    setNext(next: Logger): void {
        this.next = next;
    }
    protected forward(msg: LogMessage): void {
        this.next?.log(msg);
    }
    abstract log(msg: LogMessage): void;
}

class ErrorLogger extends BaseLogger {
    log(msg: LogMessage): void {
        // TODO: If msg.level >= 4, print "[ERROR] " + msg.message
        // TODO: Always forward to next
        if(msg.level >= 4) {
            console.log("[ERROR] " + msg.message)
        }
        this.forward(msg);
    }
}

class WarnLogger extends BaseLogger {
    log(msg: LogMessage): void {
        // TODO: If msg.level >= 3, print "[WARN] " + msg.message
        // TODO: Always forward to next
        if(msg.level >= 3) {
            console.log("[WARN] " + msg.message)
        }
        this.forward(msg);
    }
}

class InfoLogger extends BaseLogger {
    log(msg: LogMessage): void {
        // TODO: If msg.level >= 2, print "[INFO] " + msg.message
        // TODO: Always forward to next
        if(msg.level >= 2) {
            console.log("[INFO] " + msg.message)
        }
        this.forward(msg);
    }
}

class DebugLogger extends BaseLogger {
    log(msg: LogMessage): void {
        // TODO: If msg.level >= 1, print "[DEBUG] " + msg.message
        // TODO: Always forward to next
        if(msg.level >= 1) {
            console.log("[DEBUG] " + msg.message)
        }
        this.forward(msg);
    }
}

const error = new ErrorLogger(); const warn = new WarnLogger();
const info = new InfoLogger(); const debug = new DebugLogger();
error.setNext(warn); warn.setNext(info); info.setNext(debug);
error.log(new LogMessage(2, "User logged in"));
error.log(new LogMessage(4, "Database connection lost"));
