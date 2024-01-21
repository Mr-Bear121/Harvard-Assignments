-- Keep a log of any SQL queries you execute as you solve the mystery.
-- better start with what is known. I have the time and day of a crime so I should look at the report first.
select description from crime_scene_reports where year = 2021 and month = 7 and day = 28 and street = 'Humphrey Street';
-- time in which the crime was committed, '10:15 am' there were 3 witnesses. their transcipts were taken at the bakery.
-- security logs for the baker has a field for license plates. Will be valuable for later.
select activity from bakery_security_logs where year = 2021 and month = 7 and day = 28;
-- activity log is rather unimpressive. simply tells which door was used at what time
--I have an interviews table I should use it to get details of the crime
select transcript from interviews where year = 2021 and month = 7 and day = 28;
-- the thief used the atm so I should be able to look for atm transactions to match an account with the perp. the atm was on legget street.
-- thief called someone within the timeframe of the theft. he asked someone to buy him a ticket for a flight out of the country for tomorrow,"note, earliest flight out"
-- I can cross reference license plates and/or atm transactions and match them with the time of the crime to see who commited the crime
-- I dont believe that between the ATM, the bakery and the peoples table there is a relationship so ill have to manually query
select license_plate, hour, minute from bakery_security_logs where year = 2021 and month = 7 and day = 28 and hour = 10 and minute >= 10;
-- there was no license plates at 10:15 either they didnt use a car or there was a slight memory laps with the witness because I have seen a license plate at minute 14
-- lets take a look at the atm...
select atm_location from atm_transactions where year = 2021 and month = 7 and day = 28 order by atm_location;
-- several transactions at humphrey street durring this day. if I can link people with transaction that would be wonderful.
select people.name, atm_location from atm_transactions
join bank_accounts on atm_transactions.account_number = bank_accounts.account_number
join people on bank_accounts.person_id = people.id
where year = 2021 and month = 7 and day = 28 and atm_location = "Humphrey Lane" order by people.name;
-- I have a list of people who were at this atm on the day of the crime but not the time since it wasnt recorded...
-- however I know that they made a phone call and wanted to leave the next day. so I need to cross reference these names with phone numbers and plane tickets to narrow the list
-- also not that the phone call lasted less than 1 minute as the testimony stated
select people.name, phone_calls.caller, receiver, duration from phone_calls
join people on phone_calls.caller = people.phone_number
where people.name in
(
    select people.name from atm_transactions
    join bank_accounts on atm_transactions.account_number = bank_accounts.account_number
    join people on bank_accounts.person_id = people.id
    where year = 2021 and month = 7 and day = 28 and atm_location = "Humphrey Lane" order by people.name
)
and duration < 60;
-- alright, i have a list of 7 names... after looking over the tables a bit more I think I can match 1 to a license plate
select bakery_security_logs.license_plate, people.name, bakery_security_logs.hour,bakery_security_logs.minute from bakery_security_logs
join people on bakery_security_logs.license_plate = people.license_plate
where people.name in
(
    select people.name from phone_calls
    join people on phone_calls.caller = people.phone_number
    where people.name in
    (
    select people.name from atm_transactions
    join bank_accounts on atm_transactions.account_number = bank_accounts.account_number
    join people on bank_accounts.person_id = people.id
    where year = 2021 and month = 7 and day = 28 and atm_location = "Humphrey Lane" order by people.name
    )
    and duration < 60
)
and year = 2021 and month = 7 and day = 28;
-- peter was there on the day of the incident. So I need to see if there are any planes with his name on it.
select airports.full_name, people.passport_number, people.name,flights.month,flights.day from flights
join airports on flights.origin_airport_id = airports.id
join passengers on flights.id = passengers.flight_id
join people on passengers.passport_number = people.passport_number
where people.name ="Peter";
--cant be peter cause the days dont match even though he was at the airport. he was on the plan on the 27th and not 28th... I expect 29th is when he boards
select airports.full_name, people.passport_number, people.name,flights.month,flights.day from flights
join airports on flights.origin_airport_id = airports.id
join passengers on flights.id = passengers.flight_id
join people on passengers.passport_number = people.passport_number
where people.name in
(
    select people.name from atm_transactions
    join bank_accounts on atm_transactions.account_number = bank_accounts.account_number
    join people on bank_accounts.person_id = people.id
    where year = 2021 and month = 7 and day = 28 and atm_location = "Humphrey Lane" order by people.name
)
and day = 29;
-- douglas, charles and rebecca had a flight on the 29th. cross reference with the license plate query from ealier:
select bakery_security_logs.license_plate, people.name, bakery_security_logs.hour,bakery_security_logs.minute, bakery_security_logs.month, bakery_security_logs.day from bakery_security_logs
join people on bakery_security_logs.license_plate = people.license_plate
where people.name in ("Rebecca","Charles","Douglas");
-- wrong date and time
-- re-use previous with the list of 7 names mentioned ealier
select bakery_security_logs.license_plate, people.name, bakery_security_logs.hour,bakery_security_logs.minute, bakery_security_logs.month, bakery_security_logs.day from bakery_security_logs
join people on bakery_security_logs.license_plate = people.license_plate
where people.name in ("Petter","Sarah","Rebecca","Scott","Ernest","Christina","Ryan");
-- nothing matches
-- lets try something different. match license plate with names first
select people.name from people
where people.license_plate in
(
    select license_plate from bakery_security_logs where year = 2021 and month = 7 and day = 28 and hour = 10 and minute >= 10
);
--- 13 names of people between 10:10 and 11:00
-- cross refence the airport
select airports.full_name, people.passport_number, people.name,flights.month,flights.day,flights.year from flights
join airports on flights.origin_airport_id = airports.id
join passengers on flights.id = passengers.flight_id
join people on passengers.passport_number = people.passport_number
where people.name in
(
    select people.name from people
    where people.license_plate in
    (
    select license_plate from bakery_security_logs where year = 2021 and month = 7 and day = 28 and hour = 10 and minute >= 10
    )
)
and flights.month = 7 and flights.day = 29;
-- 8 flights on that day and month - diana sofia bruce kelsey taylor luca sophia thomas
-- check the atm
select people.name from atm_transactions
join bank_accounts on atm_transactions.account_number = bank_accounts.account_number
join people on bank_accounts.person_id = people.id
where people.name in
(
    "Diana", "Sofia", "Bruce", "Kelsey", "Taylor", "Luca", "Sophia", "Thomas"
)
and year = 2021 and month = 7 and day = 28 and atm_location = "Humphrey Lane" order by people.name;
-- nothing
-- give me everyone who left in the expected time range.
select airports.full_name, people.passport_number, people.name,flights.month,flights.day,flights.year from flights
join airports on flights.origin_airport_id = airports.id
join passengers on flights.id = passengers.flight_id
join people on passengers.passport_number = people.passport_number
where flights.month = 7 and flights.day = 29;
-- cross refence with the atm
select people.name from atm_transactions
join bank_accounts on atm_transactions.account_number = bank_accounts.account_number
join people on bank_accounts.person_id = people.id
where people.name in
(
    select people.name from flights
    join airports on flights.origin_airport_id = airports.id
    join passengers on flights.id = passengers.flight_id
    join people on passengers.passport_number = people.passport_number
    where flights.month = 7 and flights.day = 29
)
and year = 2021 and month = 7 and day = 28 and atm_location = "Humphrey Lane" order by people.name;
-- douglas charles rebecca.
-- were they found at the bakery at the appropriate time? "not they were found at the airport at the appropriate time and place and found at the atm"
select bakery_security_logs.license_plate, people.name, bakery_security_logs.hour,bakery_security_logs.minute, bakery_security_logs.month, bakery_security_logs.day from bakery_security_logs
join people on bakery_security_logs.license_plate = people.license_plate
where people.name in ("Rebecca","Douglas","Charles");
--douglas was on the wrong day and time
-- douglas is a good a guess as any
