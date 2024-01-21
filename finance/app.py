import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    dbData = db.execute(f"select symbol, sum(shares) as totalShares from stockTransactions where user_id={session['user_id']} group by symbol having totalShares > 0")
    tableKeys = dbData[0].keys()
    tableData = dbData
    # a list of headers refering to keys in my dict.
    tableHeaders = ["name","price","total_value"]
    userCash = 0

    dbData = db.execute(f"select cash from users where id ={session['user_id']}")
    userCash = dbData[0]["cash"]

    for item in reversed(tableKeys):
        tableHeaders.insert(0,item)

    for item in tableData:
        quote = lookup(item["symbol"])
        item["cash"] = userCash
        item["name"] = quote["name"]
        item["price"]= quote["price"]
        item["totalValue"]= item["price"] * item["totalShares"]



    return render_template("index.html", stockData=tableData, tableHeaders=tableHeaders)
    #return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    stock=""
    stocksBought=0
    userId = session['user_id']

    if request.method == "POST":

        stock=request.form.get("stockSymbol")
        stocksBought=int(request.form.get("quantity"))

        if not stock:
            return apology("Please provide a stock symbol.")
        elif not stocksBought:
            return apology("Please provide the amount of stocks you wish to purchase.")

        if stocksBought < 0:
            return apology("Invalid number of stocks")
        else:
            stockData = lookup(stock)

            if stockData == None:
                return apology("Your stock was not found.")

            stockPrice = stockData["price"]
            totalStockPrice = stockPrice * stocksBought

            query = db.execute(f"select * from users where id={userId}")
            accountCash = query[0]["cash"]

            if totalStockPrice > accountCash:
                return apology("Not enough money in your account.")
            else:
                db.execute(f"update users set cash = cash - {totalStockPrice} where id = {userId}")
                db.execute("insert into stockTransactions (user_id,symbol,shares,price) values (?,?,?,?)",userId , stock, stocksBought, totalStockPrice)
                return render_template("buy.html", confirmation="Purchase complete")
    else:
        return render_template("buy.html")
    #return apology("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    dbData = db.execute(f"select symbol,shares,price,timestamp from stockTransactions where user_id={session['user_id']} order by timestamp DESC")
    tableKeys = dbData[0].keys()
    tableData = dbData

    for item in tableData:
        quote = lookup(item["symbol"])
        item["name"] = quote["name"]

    return render_template("history.html", stockData=tableData, tableHeaders=tableKeys)

    #return apology("TODO")

@app.route("/addAccount", methods=["GET", "POST"])
@login_required
def addAccount():
    user=session["user_id"]
    account = None
    if request.method == "POST":
        account = request.form.get("accountName")

        if not account:
            flash("Please insert an account name")

        accountExists= db.execute("select accounts from accounts where accounts=?",account)

        if not accountExists:
            db.execute(f"INSERT INTO accounts (user_id,accounts) VALUES (?,?)",user,account)
            flash("Account added")
            redirect("/transfer")
        else:
            flash("This account already exists")


    return render_template("addAccount.html")

@app.route("/transfer", methods=["GET", "POST"])
@login_required
def transfer():
    user=session["user_id"]
    dbData = db.execute(f"select accounts from accounts where user_id=?",user)
    if request.method == "POST":
        accountTo = request.form.get("accountTo")
        cashAmount = request.form.get("cashAmount")
        if accountTo != "select":
            if cashAmount == "$":
                flash("Please enter an amount to transfer")
            else:
                cashAmount = int(cashAmount)
                #since this is a placeholder for a "real" transaction action all I have to do is simulate the transfer
                dbCash = db.execute(f"select cash from users where id=?",user)
                newCashAmount= int(dbCash[0].get("cash")) - cashAmount
                db.execute(f"UPDATE users SET cash=? where id=?",newCashAmount,user)
                flash("Transfer Complete")

        else:
            flash("Please select a bank.")
    return render_template("bankTransfer.html", accounts=dbData)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    stockSymbol = ""
    stockResult = {}

    if request.method == "POST":
        stockSymbol = request.form.get("symbol")
        stockResult = lookup(stockSymbol)
        if stockResult != None:
            return render_template("quoted.html", stockData=stockResult)
        else:
            return apology("I am sorry but your stock does not exist")
    else:
        return render_template("quote.html")

    #return apology("TODO")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    session.clear()

    userName = request.form.get("username")
    password = request.form.get("password")
    query = db.execute("select * from users where username=?",userName)

    if request.method == "POST":

        if not userName:
            return apology("Username must not be left blank.")
        else:
            if len(query) >= 1:
                #if the database returns a result "if it exists in the database". Then you can`t have that username
                return apology("Your username already exists.")

        if not password:
            return apology("Password must not be left blank.")
        #I had logic to check if the password was viable. but then I realised, "why do I need to check the password against the database? many users can have the same password".
        # also, if the username doesnt not exist then the usr/password combo doesnt either.

        #insert username
        db.execute("insert into users (username,hash) values(?,?)",userName,generate_password_hash(password))
        #query again to get the id associated with this username
        query = db.execute("select * from users where username=?",userName)
        session["user_id"]= query[0]["id"]
        return redirect('/')
    else:
        return render_template("register.html")
    #return apology("TODO")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    userId = session["user_id"]
    stocks = db.execute(f"select symbol, sum(shares) as totalShares from stockTransactions where user_id={userId} group by symbol having totalShares > 0")

    if request.method == "POST":

        symbol = request.form.get("symbol")
        shareQuantity = request.form.get("shareQuantity")

        if not symbol:
            return apology("Please provide a stock symbol.")

        if not shareQuantity or int(shareQuantity) <= 0:
            return apology("Please provide a positive number of shares.")
        else:
            shareQuantity = int(shareQuantity)

        for item in stocks:
            if item["symbol"] == symbol:
                if item["totalShares"] < shareQuantity:
                    return apology("Not enough shares.")
                else:
                    quote = lookup(symbol)

                    if quote is None:
                        return apology("Stock Symbol not found.")

                    price = quote["price"]
                    totalSale = shareQuantity * price

                    db.execute("update users set cash = cash + ? where id= ?", totalSale, userId)
                    db.execute("insert into stockTransactions (user_id,symbol,shares,price) values(?,?,?,?)",userId, symbol,-shareQuantity,price)

                    flash(f"sold {shareQuantity} of your stock {symbol} for {totalSale}")
                    return redirect("/")
        return apology("im sorry but we could not find your stock")
    else:
        return render_template("sell.html", stockData=stocks)
    #return apology("TODO")

if __name__ == "__main__":
    app.run(debug=True)

