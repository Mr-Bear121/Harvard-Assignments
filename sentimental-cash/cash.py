import cs50

# TODO


def main():
    cents = get_Cents()

    quarters = calculate_Quarters(cents)
    cents = cents - quarters * 25

    dimes = calculate_Dimes(cents)
    cents = cents - dimes * 10

    nickels = calculate_Nickles(cents)
    cents = cents - nickels * 5

    pennies = calculate_Pennies(cents)
    cents = cents - pennies * 1

    coinTotal = quarters + dimes + nickels + pennies
    print(f"{coinTotal}")


def get_Cents():
    cents = 0
    while cents <= 0.0:
        cents = cs50.get_float("change owed: ")
    return round(cents * 100)


def calculate_Quarters(moneyAmount):
    return int(moneyAmount / 25)


def calculate_Nickles(moneyAmount):
    return int(moneyAmount / 5)


def calculate_Dimes(moneyAmount):
    return int(moneyAmount / 10)


def calculate_Pennies(moneyAmount):
    return int(moneyAmount / 1)


if __name__ == "__main__":
    main()
