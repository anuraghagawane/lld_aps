class SqlQueryBuilder {
  private table: string;
  private columns: string[] = [];
  private conditions: string[] = [];
  private orderByCol: string | null = null;
  private orderDir: string = "ASC";
  private limitVal: number = 0;
  private offsetVal: number = 0;

  constructor(table: string) {
      this.table = table;
  }

  getTable() {
      return this.table;
  }
  getColumns() {
      return this.columns;
  }
  getConditions() {
      return this.conditions;
  }
  getOrderByCol() {
      return this.orderByCol;
  }
  getOrderDir() {
      return this.orderDir;
  }
  getLimitVal() {
      return this.limitVal;
  }
  getOffsetVal() {
      return this.offsetVal;
  }

  select(...cols: string[]): this {
      this.columns.push(...cols);
      return this;
  }

  where(condition: string): this {
      this.conditions.push(condition);
      return this;
  }

  orderBy(col: string, dir: string): this {
      this.orderByCol = col;
      this.orderDir = dir;
      return this;
  }

  limit(l: number): this {
      this.limitVal = l;
      return this;
  }
  offset(o: number): this {
      this.offsetVal = o;
      return this;
  }

  build(): SqlQuery {
      return new SqlQuery(this);
  }
};

class SqlQuery {
  private readonly table: string;
  private readonly columns: string[];
  private readonly conditions: string[];
  private readonly orderByCol: string | null;
  private readonly orderDir: string;
  private readonly limitVal: number;
  private readonly offsetVal: number;

  constructor(builder: SqlQueryBuilder) {
    this.table = builder.getTable();
    this.columns = [...builder.getColumns()];
    this.conditions = [...builder.getConditions()];
    this.orderByCol = builder.getOrderByCol();
    this.orderDir = builder.getOrderDir();
    this.limitVal = builder.getLimitVal();
    this.offsetVal = builder.getOffsetVal();
  }

  toSql(): string {
    const cols = this.columns.length > 0 ? this.columns.join(", ") : "*";
    let sql = `SELECT ${cols} FROM ${this.table}`;
    if (this.conditions.length > 0) sql += ` WHERE ${this.conditions.join(" AND ")}`;
    if (this.orderByCol) sql += ` ORDER BY ${this.orderByCol} ${this.orderDir}`;
    if (this.limitVal > 0) sql += ` LIMIT ${this.limitVal}`;
    if (this.offsetVal > 0) sql += ` OFFSET ${this.offsetVal}`;
    return sql;
  }

  static Builder = SqlQueryBuilder;
}

const query1 = new SqlQuery.Builder("users")
    .select("name", "email")
    .where("age > 18")
    .where("active = true")
    .orderBy("name", "ASC")
    .limit(10)
    .build();

const query2 = new SqlQuery.Builder("orders")
    .select("id", "total", "created_at")
    .where("status = 'completed'")
    .where("total > 100")
    .orderBy("created_at", "DESC")
    .limit(20)
    .offset(40)
    .build();

console.log(query1.toSql());
console.log(query2.toSql());
