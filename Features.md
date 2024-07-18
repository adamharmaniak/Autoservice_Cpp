# Features
This application is an object-oriented program with a user interface designed to manage car service orders. The system has the following *features*:

## Users
### User Groups:
- *Individual*  
- *Company*  
- *Car Service Worker (Technician and Administrator)*  

## User Credentials:
- **Login name**  
- **Login password**  
### Additional details:
- *Individuals and Companies:* Full name/company name, car information (make, model, year)
- *Only Companies:* Registration number, VAT registration status
- *Car Service Worker:* Job position (technician or administrator)

## User Database:
- **Stored in files**  
- **Loaded automatically at program startup**  
- **Main Functionalities**  

## Service Management:
- Services and their prices are loaded from a file
- Administrators can update this file through the UI

## User Authentication:
- Login window to select user and enter a password
- Error message for incorrect passwords

## User Actions (for Individuals and Companies):
- Edit car records
- Order services from the garage
- Cancel orders
- View list of orders with prices (VAT details for companies)
- Check order status (new/finished/completed)
- Generate invoice text file with total prices
- Log out

## Order Management:
- Orders are saved in files to retain data between sessions
- Orders file updated after user logout

## Administrator Privileges:
- Add, edit, or delete all users

## Technician and Administrator Actions:
- View list of orders
- Update order status to pending/completed
- Prevent users from canceling completed orders
