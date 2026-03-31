class RecipientList {
    to: string[];
    cc: string[];

    constructor(to: string[], cc: string[]) {
        this.to = [...to];
        this.cc = [...cc];
    }

    deepCopy(): RecipientList {
        return new RecipientList([...this.to], [...this.cc]);
    }
}

class EmailTemplate {
    subject: string;
    body: string;
    recipients: RecipientList;

    constructor(subject: string, body: string, recipients: RecipientList) {
        this.subject = subject;
        this.body = body;
        this.recipients = recipients;
    }

    clone(): EmailTemplate {
        return new EmailTemplate(this.subject, this.body, this.recipients.deepCopy());
    }

    print(): void {
        console.log(`Email: ${this.subject} | Recipients: ` +
                     `{to=[${this.recipients.to.join(", ")}], ` +
                     `cc=[${this.recipients.cc.join(", ")}]}`);
    }
}

const baseRecipients = new RecipientList(["all@company.com"], ["archive@company.com"]);
const baseTemplate = new EmailTemplate(
    "Company Newsletter", "Monthly updates from the team...", baseRecipients);

const marketingEmail = baseTemplate.clone();
marketingEmail.subject = "Marketing Newsletter";
marketingEmail.recipients.to.push("marketing@company.com");

const engineeringEmail = baseTemplate.clone();
engineeringEmail.subject = "Engineering Newsletter";
engineeringEmail.recipients.to.push("eng-team@company.com");

const hrEmail = baseTemplate.clone();
hrEmail.subject = "HR Newsletter";
hrEmail.recipients.to.push("hr@company.com");
hrEmail.recipients.cc.push("ceo@company.com");

marketingEmail.print();
engineeringEmail.print();
hrEmail.print();

console.log("\nBase template unchanged:");
baseTemplate.print();
