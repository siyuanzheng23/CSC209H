# Simple Social App
A simple command-line based social app implemented by C.<br>
Mark got for this assignment: Around 81.25%, which is above class average.
<br><br>

To launch the app:<br>
1, open terminal and cd into the directory after cloned locally.<br>
2, `make` , it should generates a file called friendme, or tells you that it is up to date.<br>
3, `friendme`.<br><br><br>

Command Descriptions:<br><br>

`add_user   username`<br>
Adds a new user to the data structure. The username must be unique.<br><br>

`delete_user   username`<br>
Delete this user from the data structure. Remove this user from any friend's lists. Do not remove the old posts from this user on old
friend's profiles.<br><br>

`list_users`<br>
Prints to standard output the usernames of all the current users. This should be printed in the order in which the users were added to the
system. The most recently-added user is printed last.<br><br>

`make_friends   user1   user2`<br>
Make a friendship between user1 and user2.<br><br>

`post   author   target   msgpiece [msgpiece ...]`<br>
Post this message to the profile of user target from user author.<br><br>

`profile   username`<br>
Display the profile of this user.<br><br>

`update_pic  username   filename`<br>
Add (or replace) the filename for the profile picture that will be displayed for this user.<br><br>

`quit` <br>
Closes the friendme program.<br><br>
