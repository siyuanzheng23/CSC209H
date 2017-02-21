#include "friends.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>


int is_fds_or_not(User * firstone,User * secondone){
	User ** fs = firstone -> friends;
	for(int m = 0; m < MAX_FRIENDS; m++){  /*checking if name2 is name1's fd */
        if(fs[m] != NULL){
            if(strcmp(fs[m] -> name, secondone -> name) == 0){
                return 0;
            }
        }
     }
     return 1;
 }
	



User * helper_of_creater_user(const char *name){ /* helper function for creating a new User */
    User *new_user = malloc(sizeof(User));
    strcpy(new_user -> name, name);
    new_user -> next = NULL;
    return new_user;
}



User * get_user_by_name(const char *name,User * head){  /* helper function which returns the pointer of the user's name is name */
    User *curr = head;
    while(curr != NULL){
        if(strcmp(curr -> name, name) == 0){
            return curr;
        }
        curr = curr -> next;
    }
    return NULL;
}

/*
 * Create a new user with the given name.  Insert it at the tail of the list
 * of users whose head is pointed to by *user_ptr_add.
 *
 * Return:
 *   - 0 if successful
 *   - 1 if a user by this name already exists in this list
 *   - 2 if the given name cannot fit in the 'name' array
 *       (don't forget about the null terminator)
 */

int create_user(const char *name, User **user_ptr_add){
    
    User *curr = *user_ptr_add;
    User *anothercurr = *user_ptr_add;

    while(curr != NULL){
        if(strcmp(name,curr->name) == 0)  /* name is a pointer here so no '&' is needed */
        {
            return 1;
        }
        curr = curr -> next;
    }
    
    if(strlen(name) > (MAX_NAME - 1)){
        return 2;
    }
    else{
    	if(*user_ptr_add == NULL){
    		*user_ptr_add = helper_of_creater_user(name);
    		return 0;
		}
        else if(*user_ptr_add != NULL){
        	while(anothercurr -> next != NULL){
        		anothercurr = anothercurr -> next;
        	}
        	User * newuser = helper_of_creater_user(name);
        	anothercurr -> next = newuser;
        	return 0;
        }
        return 0;
    }
}

/*
 * Return a pointer to the user with this name in
 * the list starting with head. Return NULL if no such user exists.
 *
 * NOTE: You'll likely need to cast a (const User *) to a (User *)
 * to satisfy the prototype without warnings.
 */
User *find_user(const char *name, const User *head) {
    User *curr = (User *)head;

    while(curr != NULL){
        if(strcmp(name,curr->name) == 0){
            return curr;
        }
        curr = curr -> next;
    }
    
    return NULL;
}


/*
 * Print the usernames of all users in the list starting at curr.
 * Names should be printed to standard output, one per line.
 */
void list_users(const User *curr) {
    while(curr != NULL){
        printf("%s\n",curr -> name);
        curr = curr -> next;
    }
}

/*
 * Change the filename for the profile pic of the given user.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if the file does not exist.
 *   - 2 if the filename is too long.
 */
int update_pic(User *user, const char *filename) {
    if(strlen(filename) > (MAX_NAME -1) ){
        return 2;
    }
    else{
        strncpy(user -> profile_pic, filename, MAX_NAME);
        return 0;
    }
}

/*
 * Make two users friends with each other.  This is symmetric - a pointer to
 * each user must be stored in the 'friends' array of the other.
 *
 * New friends must be added in the first empty spot in the 'friends' array.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if the two users are already friends.
 *   - 2 if the users are not already friends, but at least one already has
 *     MAX_FRIENDS friends.
 *   - 3 if the same user is passed in twice.
 *   - 4 if at least one user does not exist.
 *
 * Do not modify either user if the result is a failure.
 * NOTE: If multiple errors apply, return the *largest* error code that applies.
 */
int make_friends(const char *name1, const char *name2, User *head) {
    User *curr = head;
    User * first = get_user_by_name(name1,head);
    User * second = get_user_by_name(name2,head);
	User ** temp1 = first -> friends;
    User ** temp2 = second -> friends;
    int exist_or_not = 0;

    if(strcmp(name1,name2) == 0){
        return 3;
    }
    
    while(curr != NULL){
        if(strcmp(curr -> name, name1) == 0){
            exist_or_not += 1;
        }
        else if(strcmp(curr -> name, name2) == 0){
            exist_or_not += 1;
        }
        curr = curr -> next;
    }
    
    if(exist_or_not < 2){
        return 4;
    }
    
    int count1 = 0, count2 = 0;
   
    for(int i = 0; i < MAX_FRIENDS ; i++){ /* checking 1st's fd list */
    	if(temp1[i] !=  NULL){
            count1 += 1;
        }
        else{
        	break;
        }

        if(count1 == 10){
        	return 2;
        }
    }
    
    for(int j = 0; j < MAX_FRIENDS ; j++){ /* checking 2nd's fd list*/
    	if(temp2[j] != NULL){
    		count2 += 1;
    	}
    	else{
    		break;
    	}

    	if(count2 == 10){
    		return 2;
    	}
    }
    
    for(int m = 0; m < MAX_FRIENDS; m++){  /*checking if name2 is name1's fd */
    	if(temp1[m] != NULL){
    		if(strcmp(temp1[m] -> name, name2) == 0){
    			return 1;
    		}
    	}
     }

    int i = 0, j = 0;
    while(temp1[i] != NULL){
    	i ++;
    }
    temp1[i] = second;
    while(temp2[j] != NULL){
    	j ++;
    }
    temp2[j] = first;

    return 0;
}

/*
 * Print a user profile.
 * For an example of the required output format, see the example output
 * linked from the handout.
 * Return:
 *   - 0 on success.
 *   - 1 if the user is NULL.
 */
int print_user(const User *user) {
    printf("profile  %s\n",user->name);
    printf("Name:  %s\n",user->name);
    printf("\n");
    printf("_______________________\n");
    printf("Friends:\n");
    for(int i =0; i < MAX_FRIENDS; i++){
    	if((user -> friends)[i] != NULL){
        	printf("%s\n",(user -> friends)[i] -> name);
        }
    }
    printf("_______________________\n");
    printf("Posts:\n");
    Post  *post = user -> first_post;
    while(post != NULL){
        printf("From:%s\n",post -> author);
        printf("Date:%s\n",ctime(post -> date));
        printf("%s\n\n",post -> contents);
        post = post -> next;
    }
    
    return 0;
}

/*
 * Make a new post from 'author' to the 'target' user,
 * containing the given contents, IF the users are friends.
 *
 * Insert the new post at the *front* of the user's list of posts.
 *
 * Use the 'time' function to store the current time.
 *
 * 'contents' is a pointer to heap-allocated memory - you do not need
 * to allocate more memory to store the contents of the post.
 *
 * Return:
 *   - 0 on success
 *   - 1 if users exist but are not friends
 *   - 2 if either User pointer is NULL
 */
int make_post(const User *author, User *target, char *contents) {
    User * a = (User *)author;
    User * t = target;
    
    if( a == NULL || t == NULL){
    	return 2;
    }

    Post * p  = target -> first_post;

    int h = is_fds_or_not(a,t);

    if(h == 1){
    	return 1;
    }
    else{
    	Post * newpost = malloc(sizeof(Post));
    	newpost -> date = malloc(sizeof(time_t));
        *(newpost -> date) = time(NULL);
    	newpost -> contents = contents;
    	strcpy(newpost -> author, a -> name);

    	if(p != NULL){
    		newpost -> next = p;
            t -> first_post = newpost;
    	}
    	else if(p == NULL){
    		t -> first_post = newpost;
    	}
    }

    	return 0;
}



/*
 * From the list pointed to by *user_ptr_del, delete the user
 * with the given name.
 * Remove the deleted user from any lists of friends.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if a user with this name does not exist.
 */
int delete_user(const char *name, User **user_ptr_del) {

    User * curr = * user_ptr_del;
    if(find_user(name,curr) == NULL){
        return 1;
    }
    else{
        while(curr != NULL){
            for(int i = 0; i < MAX_FRIENDS; i++){
                if(curr -> friends[i] != NULL){
                    if (strcmp(curr -> friends[i] -> name , name) == 0){
                        (curr -> friends)[i] = NULL;
                        /* should I put a break statement here */
                    }
                 }
            }
            curr = curr -> next;
        }

        curr = *user_ptr_del;
        while(curr != NULL){
            if(curr -> next != NULL && strcmp(curr -> next -> name, name) == 0 && curr -> next -> next != NULL){ /* deleting middle user */
                User * former = curr;
                User *later = curr -> next -> next;
                Post * post_to_be_deleted = curr-> next -> first_post;
                Post * next_post = NULL;
                while(post_to_be_deleted != NULL){  /* deallocating the memory of every post here */
                    next_post = post_to_be_deleted -> next;
                    free(post_to_be_deleted -> date);     /* deallocate date */
                    free(post_to_be_deleted);            /* deallocate post */
                    post_to_be_deleted = next_post;
                }
                free(curr -> next);
                former -> next = later;
                return 0;
            }
            else if(curr -> next != NULL && strcmp(curr -> next -> name, name) == 0 && curr -> next -> next == NULL){/*the user is at the end*/
                Post * post_to_be_deleted = curr -> next -> first_post;
                Post * next_post = NULL;
                while(post_to_be_deleted != NULL){  /* deallocating the memory of every post here */
                    next_post = post_to_be_deleted -> next;
                    free(post_to_be_deleted -> date);
                    free(post_to_be_deleted);
                    post_to_be_deleted = next_post;
                }
                free(curr->next);
                curr -> next = NULL;
                return 0;
            }
            else if(curr != NULL && strcmp(curr -> name, name) == 0){  /* deleting the first user */
                *user_ptr_del = curr -> next;
                Post * post_to_be_deleted = curr ->  first_post;
                Post * next_post = NULL;
                while(post_to_be_deleted != NULL){  /* deallocating the memory of every post here */
                    next_post = post_to_be_deleted -> next;
                    free(post_to_be_deleted -> date);
                    free(post_to_be_deleted);
                    post_to_be_deleted = next_post;
                }
                free(curr);
                return 0;
            }
            else if(curr != NULL && curr -> next == NULL && strcmp(curr -> name,name) == 0){
                Post * post_to_be_deleted = curr ->  first_post;
                Post * next_post = NULL;
                while(post_to_be_deleted != NULL){  /* deallocating the memory of every post here */
                    next_post = post_to_be_deleted -> next;
                    free(post_to_be_deleted -> date);
                    free(post_to_be_deleted);
                    post_to_be_deleted = next_post;
                }
                free(curr);
                *user_ptr_del = NULL;
            }

            curr = curr -> next;
        }
        
        
        
    }
    
    return 0;
}