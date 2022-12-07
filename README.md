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
