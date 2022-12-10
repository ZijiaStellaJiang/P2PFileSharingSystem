# ECE558_P2P_File_Sharing_F22

## Getting started

This project mainly implemented in Linux with C++. To start running the project, please make sure you followed the instruction below.

## Database: PostgreSQL

#### Install PostgreSQL 

        sudo apt install postgresql
        sudo apt install postgresql-contrib

#### Install C++ API 

        sudo apt install libpqxx-dev

#### Create Database (first time)

First, change the username to postgres username:

        sudo su - postgres

Then get into postgres:

        psql

Alter the user with password

        ALTER USER postgres with encrypted password 'postgres';

Then quit the postgres with command '\q' and 'exit'

###### Update the pg_hba.conf file (first time)

Open the pg_hba.conf file and update it, note, in this project we are using postgres version 12:

        sudo vim /etc/postgresql/12/main/pg_hba.conf

Change the line 

        local all         postgres           peer

To:
    
        local all         postgres           md5

###### Restart postgres: 

        sudo service postgresql restart

###### Create the database 

        CREATE DATABASE server_db;

## Protocol: Google Protocol Buffers

        sudo apt-get install libprotobuf-dev protobuf-compiler

## Run the Project 

#### Server

In ./server enter 

        make

After make success
        
        ./main

You can also check the database while running the server by:

        psql -U postgres

enter the password "postgres", then

        \c server_db

then 

        SELECT * FROM file;

#### Client

in ./client directory, run

        make

then run,

        ./main <your server machine ip address>

Then follow the stdout on terminal to work!

#### Reminder

Please do not run server and client on same machine.

Port number should be 4 digits int.

IP address should follow the format 127.0.0.1