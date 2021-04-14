# Aplhabetical
### Quiz game where players must answer a set of questions based on the letters of the alphabet!

## Topics involved:
- Binary search trees
- Circular linked lists
- File handling
- Struct implementation

## Steps to get started:
**Prerequisites:** A local web server installed. I will use [Xampp](https://www.apachefriends.org/es/index.html) in this example
which is the most famous but you could use any other. [Some examples for you here](https://www.emezeta.com/articulos/15-aplicaciones-para-montar-servidores-web-en-local)
- Clone the repo in the htdocs folder of your Xampp download. The path should be like ``C:\xampp\htdocs\yourCutePage``
- Open Xampp and click *Start* of Apache and MySql
- Open your broser and go to ``localhost\phpMyAdmin``. There, click on the **Import** tab and click **Examine**. A window
will open and you must select the ``electrocom.sql`` file that is in the file you cloned, in the **Extras** folder. This
will relieve you from having to create the database tables by yourself
- In your browser, go to ``localhost\yourCutePage``
- Have fun with the marvels of programming!

**Note:** The starting admin user is ***iamroot*** and its password is ***pass***.
The other users are **Kratos**, **Magnolia**, **Felipe**, **Ralph** and **Hermione**, and their passwords are ***1234***

## Actions in the page:
### The page allows the user to do actions according to their authorization levels

If the user enters as visitor, it will be able to:
- See the categories and details of models
- See the comments of a specific model
- Sort comments by user, date or score

If the user is logged in, it could:
- Perform the same actions of a visitor
- Add a score and a comment for a product model

If the user is logged in with the admin account, it will be able to:
- Do the same things a common user can
- Delete comments
- Add, delete and modify categories
- Add, delete and modify models as well
- See and delete user accounts
- Give and remove admin permission to other users
