interface Document {
  getHeader(): string;
  formatRow(data: string[]): string;
  getFooter(): string;
  getFileExtension(): string;
}

class PdfDocument implements Document {
  getHeader(): string {
      return "--- PDF DOCUMENT START ---";
  }
  formatRow(data: string[]): string {
      return "| " + data.join(" | ") + " |";
  }
  getFooter(): string {
      return "--- PDF DOCUMENT END ---";
  }
  getFileExtension(): string {
      return ".pdf";
  }
}

class HtmlDocument implements Document {
    getHeader(): string {
        return "<html><body><table>";
    }
    formatRow(data: string[]): string {
        const cells = data.map(cell => `<td>${cell}</td>`).join("");
        return `<tr>${cells}</tr>`;
    }
    getFooter(): string {
        return "</table></body></html>";
    }
    getFileExtension(): string {
        return ".html";
    }
}

class CsvDocument implements Document {
    getHeader(): string {
        return "";
    }
    formatRow(data: string[]): string {
        return data.join(",");
    }
    getFooter(): string {
        return "";
    }
    getFileExtension(): string {
        return ".csv";
    }
}

abstract class ExportCreator {
  abstract createDocument(): Document;

  export(data: string[][]): void {
    const doc = this.createDocument();
    console.log(`Exporting to ${doc.getFileExtension()} format...`);

    const header = doc.getHeader();
    if (header) console.log(header);

    for (const row of data) {
        console.log(doc.formatRow(row));
    }

    const footer = doc.getFooter();
    if (footer) console.log(footer);

    console.log("Export complete.\n");
  }
};

class PdfExportCreator extends ExportCreator {
    createDocument(): Document {
        return new PdfDocument();
    }
}

class HtmlExportCreator extends ExportCreator {
    createDocument(): Document {
        return new HtmlDocument();
    }
}

class CsvExportCreator extends ExportCreator {
    createDocument(): Document {
        return new CsvDocument();
    }
}

function main(): void {
    const reportData: string[][] = [
        ["Name", "Department", "Salary"],
        ["Alice", "Engineering", "120000"],
        ["Bob", "Marketing", "95000"],
        ["Charlie", "Design", "105000"],
    ];

    const pdfExporter = new PdfExportCreator();
    pdfExporter.export(reportData);

    const htmlExporter = new HtmlExportCreator();
    htmlExporter.export(reportData);

    const csvExporter = new CsvExportCreator();
    csvExporter.export(reportData);
}

main();
