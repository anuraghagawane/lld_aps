class Email {
    readonly to: string;
    readonly subject: string;
    readonly cc: string[];
    readonly bcc: string[];
    readonly body: string;
    readonly priority: string;
    readonly attachments: string[];

    private constructor(builder: InstanceType<typeof Email.Builder>) {
        this.to = builder.toAddr;
        this.subject = builder.subjectText;
        this.cc = [...builder.ccList];
        this.bcc = [...builder.bccList];
        this.body = builder.bodyText;
        this.priority = builder.priorityText;
        this.attachments = [...builder.attachmentList];
    }

    toString(): string {
        // TODO: Return formatted string showing all fields
        // Expected format: Email{to='...', subject='...', cc=[...], bcc=[...], body='...', priority='...', attachments=[...]}
        return `Email{to='${this.to}', subject='${this.subject}', cc=[${this.cc.join(", ")}], bcc=[${this.bcc.join(", ")}], body='${this.body}', priority='${this.priority}', attachments=[${this.attachments.join(", ")}]}`;
    }

    static Builder = class {
        toAddr: string;
        subjectText: string;
        ccList: string[] = [];
        bccList: string[] = [];
        bodyText: string = "";
        priorityText: string = "normal";
        attachmentList: string[] = [];

        constructor(to: string, subject: string) {
            this.toAddr = to;
            this.subjectText = subject;
        }

        cc(cc: string): this {
            // TODO: Push cc to ccList
            this.ccList.push(cc);
            return this;
        }

        bcc(bcc: string): this {
            // TODO: Push bcc to bccList
            this.bccList.push(bcc);
            return this;
        }

        setBody(body: string): this {
            // TODO: Set the bodyText field
            this.bodyText = body;
            return this;
        }

        setPriority(priority: string): this {
            // TODO: Set the priorityText field
            this.priorityText = priority;
            return this;
        }

        attachment(attachment: string): this {
            // TODO: Push attachment to attachmentList
            this.attachmentList.push(attachment);
            return this;
        }

        build(): Email {
            return new Email(this);
        }
    };
}

const email1 = new Email.Builder("alice@example.com", "Meeting Tomorrow")
        .setBody("Let's meet at 10am in conference room B.")
        .build();

const email2 = new Email.Builder("bob@example.com", "Project Update")
        .cc("carol@example.com")
        .cc("dave@example.com")
        .bcc("manager@example.com")
        .setBody("Attached is the Q4 report.")
        .setPriority("high")
        .attachment("q4-report.pdf")
        .attachment("summary.xlsx")
        .build();

console.log(email1.toString());
console.log();
console.log(email2.toString());
