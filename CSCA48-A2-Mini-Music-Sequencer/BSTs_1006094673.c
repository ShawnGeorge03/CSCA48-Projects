#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NoteSynth.c"

typedef struct BST_Node_Struct
{
    // This compound type stores all data for one node of the
    // BST. Since the BST is used to store musical notes,
    // the data contained here represents one note from a
    // musical score:
    // freq: A double-precision floating point value,
    //       corresponding to the frequency (pitch) of the note
    // bar: Musical scores are divided into 'bars' (which you can
    //      see are actually separated by a vertical bar!). This
    //      value indicates which bar the note happens in. The
    //      first bar in the musical score is 0
    // index: Position of the note within the bar, from 0 (at the
    //        beginning of the bar) to 1 (at the end of the bar)
    // key: A unique identifier (remember we discussed BST nodes
    //      should have unique keys to identify each node). We
    //      want our nodes to store notes in the order in which
    //      they occur in the song. So, the key identifier is
    //      created as: key = (10.0*bar)+index
    //      NOTE: This means only one note can have a specific
    //            bar,index value. If two notes should happen
    //            at the same time in the song, we make the
    //            index of one of them a tiny bit bigger or
    //            a tiny bit smaller than the other one so
    //            their keys are very close, but not identical.

    double key;
    double freq;
    int bar;
    double index;
    /*** TO DO:
     * Complete the definition of the BST_Node_Struct
     ***/
     struct BST_Node_Struct *left;
     struct BST_Node_Struct *right;

} BST_Node;

BST_Node *newBST_Node(double freq, int bar, double index)
{
    /*
     * This function creates and initializes a new BST_Node
     * for a note with the given position (bar:index) and
     * the specified frequency. The key value for the node
     * is computed here as
     */

     BST_Node *temp = (BST_Node *)calloc(1, sizeof(BST_Node));

     if(temp != NULL && (index >= 0.0 && index <= 1.0) && bar >= 0 && freq > 0){
       temp->key = (10.0*bar)+index;
       temp->freq =freq;
       temp->bar = bar;
       temp->index = index;
       temp->left = NULL;
       temp->right = NULL;
       return temp;
     }

    return NULL;
}

BST_Node *BST_insert(BST_Node *root, BST_Node *new_node)
{
    /*
     * This function inserts a new node into the BST. The
     * node must already have been initialized with valid
     * note data, and must have its unique key.
     *
     * The insert function must check that no other node
     * exists in the BST with the same key. If a node with
     * the same key exists, it must print out a message
     * using the following format string
     *
     * printf("Duplicate node requested (bar:index)=%d,%lf, it was ignored\n",....);
     * (of course you need to provide the relevant variables to print)
     *
     * And it must return without inserting anyting in the
     * BST.
     */

     if(root == NULL) return new_node;

     if(root->key == new_node->key){
       printf("Duplicate node requested (bar:index)=%d,%lf, it was ignored\n",root->bar,root->index);
     }else if(root->key > new_node->key){
       root->left = BST_insert(root->left,new_node);
     }else{
       root->right = BST_insert(root->right,new_node);
     }
     return root;
}

BST_Node *BST_search(BST_Node *root, int bar, double index)
{
    /*
     * This function searches the BST for a note at the
     * speficied position. If found, it must return a
     * pointer to the node that contains it.
     * Search has to happen according to the BST search
     * process - so you need to figure out what value to
     * use during the search process to decide which branch
     * of the tree to search next.
     */

     if(root == NULL) return NULL;

     double key =  (10.0*bar)+index;

     if(root->key == key) return root;

     if(root->key > key){
       return BST_search(root->left, bar, index);
     }else{
       return BST_search(root->right, bar, index);
     }
}

BST_Node *find_successor(BST_Node *right_child_node)
{
    /*
     * This function finds the successor of a node by
     * searching the right subtree for the node that
     * is most to the left (that will be the node
     * with the smallest key in that subtree)
     */

     if(right_child_node->left != NULL){
       return find_successor(right_child_node->left);
     }
     return right_child_node;
}

BST_Node *BST_delete(BST_Node *root, int bar, double index)
{
    /*
     * Deletes from the BST a note at the specified position.
     * You must implement the three cases of BST deletion
     * we discussed in class. Make sure the function can
     * remove a note at any position without breaking the
     * tree!
     */

     BST_Node *temp;
     if(root == NULL) return root;

     double key = (10.0*bar) + index;
     if(root->key == key){
       if(root->left == NULL && root->right == NULL){
         free(root);
         return NULL;
       }else if(root->left == NULL){
         temp = root->right;
         free(root);
         return temp;
       }else if(root->right == NULL){
         temp = root->left;
         free(root);
         return temp;
       }else{
         temp = find_successor(root->right);
         root->key = temp->key;
         root->freq = temp->freq;
         root->bar = temp->bar;
         root->index = temp->index;
         root->right = BST_delete(root->right, temp->bar, temp->index);
       }
     }else if(key < root->key){
       root->left = BST_delete(root->left, bar, index);
     }else if(key > root->key){
       root->right = BST_delete(root->right, bar, index);
     }
     return root;
}

void BST_makePlayList(BST_Node *root)
{
    /*
     * This function does an in-order traversal of the BST to
     * generate an ordered list of notes to be played. Each
     * note is added to a linked-list (already implemented,
     * you only need to call the insert function) and the
     * play list is then playable using the code in NoteSynth.c
     *
     * To insert a note, you need to call the function provided
     * in NoteSynth.c:
     *
     * playlist_head=playlist_insert(freq,bar,index);
     *
     * playlist_head is a GLOBAL variable declared in NoteSynth.c
     * precisely for this purpose. Don't worry about intializing
     * it. It's set to NULL.
     *
     * playlist_insert() takes the frequency, bar, and index, and
     * adds the note to the the *end* of the list - so notes
     * have to be added in order - hence the in-order traversal
     * this function has to do.
     */

     if(root == NULL){
       return;
     }

     BST_makePlayList(root->left);
     playlist_head=playlist_insert(playlist_head,root->freq,root->bar,root->index);
     BST_makePlayList(root->right);



}

void BST_inOrder(BST_Node *root, int depth)
{
    /*
     * This function performs an in-order traversal of the BST
     * and prints out the note information for each note
     * using this print statement:
     *
     *  printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n",...);
     *
     * Obviously, you must provide the bar, index, and frequency
     * of the note that needs to be printed to complete the
     * statement - we're just giving you the formatting string.
     *
     * The depth value is increased by 1 for each recursive call
     * so when you print, you can see at what level each node
     * is located! (this should help you debug your code by
     * making it easier to check the shape of your BST).
     */

     if(root == NULL){
       return;
     }else{
       depth++;
     }

     BST_inOrder(root->left, depth);
     printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth - 1, root->bar, root->index, root->freq);
     BST_inOrder(root->right, depth);

}

void BST_preOrder(BST_Node *root, int depth)
{
    /*
     * This function performs an pre-order traversal of the BST
     * and prints out the note information for each note
     * using this print statement:
     *
     *  printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n",...);
     *
     * Obviously, you must provide the bar, index, and frequency
     * of the note that needs to be printed to complete the
     * statement - we're just giving you the formatting string.
     *
     * The depth value is increased by 1 for each recursive call
     * so when you print, you can see at what level each node
     * is located! (this should help you debug your code by
     * making it easier to check the shape of your BST).
     */

     if(root == NULL){
       return;
     }else{
       depth++;
     }

     printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth - 1, root->bar, root->index, root->freq);
     BST_preOrder(root->left, depth);
     BST_preOrder(root->right, depth);

}

void BST_postOrder(BST_Node *root,int depth)
{
    /*
     * This function performs an post-order traversal of the BST
     * and prints out the note information for each note
     * using this print statement:
     *
     *  printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n",...);
     *
     * Obviously, you must provide the bar, index, and frequency
     * of the note that needs to be printed to complete the
     * statement - we're just giving you the formatting string.
     *
     * The depth value is increased by 1 for each recursive call
     * so when you print, you can see at what level each node
     * is located! (this should help you debug your code by
     * making it easier to check the shape of your BST).
     */

     if(root == NULL){
       return;
     }else{
       depth++;
     }

     BST_postOrder(root->left, depth);
     BST_postOrder(root->right, depth);
     printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth - 1, root->bar, root->index, root->freq);

}

void delete_BST(BST_Node *root)
{
    /*
     * This function deletes the BST and frees all memory used for
     * nodes in it. Recall that there is a specific order in which
     * this needs to be done! (consult the Unit 4 notes as needed)
     */

     if(root == NULL){
       return;
     }

     delete_BST(root->left);
     delete_BST(root->right);
     free(root);
}

void BST_shiftFreq(BST_Node *root, char note_src[5], char note_dst[5])
{
    /*
     * This function goes over the entries in the BST, finds notes
     * corresponding to note_src (which is a text string describing
     * a note's name in the musical scale), and shifts those notes
     * to the frequency of note_dst (another text string with a note
     * name). Effectively, it replaces one particular type of note
     * by another all through the song.
     *
     * The note names are in either 2 or 3 characters long, and
     * you can see all of them in the file 'note_frequencies.txt'
     *
     * Happily, you *DON'T HAVE TO READ THAT FILE YOURSELVES*, the
     * NoteSynth code has already done that, and it gives you
     * two GLOBAL arrays:
     *
     * char note_names[100][5] - an array of 100 rows, for strings
     *                           with at most 5 characters each.
     * 				 Each note name is one row in this
     * 				 array and the notes are sorted
     * 				 by frequency.
     * 				 There are 100 notes in all
     * double note_freq[100]   - An array of double precision
     * 				 floating point numbers with the
     * 				 frequencies for each of the
     * 				 notes in note_names[][].
     * 				 note_freq[i] has the frequency
     * 				 for the note whose name is
     * 				 stored in note_names[i][]
     *
     * The user doesn't know about frequencies, so they will just
     * give you the note names they want. If the user sends a
     * string that doesn't correspond to a valid note name, for
     * either the note_src[] or the note_dst[], then this
     * function should do nothing.
     *
     * Expected result: All notes in the BST whose frequency
     * corresponds to the frequency of note_src, should have
     * their frequency changed to that of note_dst.
     */

     double src = 0;
     double dst = 0;

     for(int i = 0; i < 100; i++){
       if(strcmp(note_names[i], note_src) == 0){
         src = note_freq[i];
       }else if(strcmp(note_names[i], note_dst) == 0){
         dst = note_freq[i];
       }
     }

     if (root != NULL){
       BST_shiftFreq(root->left, note_src, note_dst);
       if (src==root->freq) root->freq = dst;
       BST_shiftFreq(root->right, note_src, note_dst);
     }
}

void harmonizer(BST_Node *head, BST_Node *curr, int semitones, double time_shift){

  if(curr != NULL){
    harmonizer(head,curr->left,semitones,time_shift);
    double newFreq = 0;
    BST_Node *new_note = NULL;

    //Finds the new Frequency
    for(int i = 0; i < 100; i++){
      if((note_freq[i] == curr->freq) && (i + semitones >= 0) && (i + semitones < 100)){
        newFreq = note_freq[i + semitones];
        break;
      }
    }

    //Creates a node
    if((newFreq != 0) && (curr->index + time_shift >= 0) &&(curr->index + time_shift < 1)){
      new_note = newBST_Node(newFreq, curr->bar, curr->index + time_shift);
      if(BST_search(head, new_note->bar, new_note->index) != NULL){
        new_note->index += 0.00000001;
      }
      head = BST_insert(head, new_note);
    }
    harmonizer(head,curr->right,semitones,time_shift);
  }
}

BST_Node *BST_harmonize(BST_Node *root, int semitones, double time_shift)
{
    /* Let's play with notes, because we can.
     *
     * This function traverses the BST, and for each existing
     * note, inserts a new, modified note (i.e. it will add sounds
     * to an already existing song, based on the notes it already has)
     *
     * The new note has the followin properties:
     * - The frequency is shifted by the specified number of semitones
     *   (A semitone is the difference between one note and the
     *    immediately next one in the musical scale - ot what is the
     *    same, the difference in pitch between a white key and the
     *    black key immediately next to it in a piano)
     * - It plays in the same *bar* as the original note
     * - But its *index* is shifted by the specified time_shift
     *   (this value is between 0 and 1, but you have to check
     *    that the final index remains between 0 and 1)
     *
     * Both the 'semitones' and 'time_shift' parameter can be
     * positive or negative. A typical value for semitones
     * could be 4 or 7, corresponding to musical 3rds or
     * musical 5ths - this should produce some interesting
     * harmony! but you can play with this function and try
     * various things for fun.
     *
     * In practice, figuring out the frequency of the note
     * you have to insert is easy. For instance, suppose
     * you have an existing note in  the BST with
     *
     * freq=440.0, at bar=10, index=.25
     *
     * Now suppose the user specified
     *
     * semitones=4
     * time_shift=.1
     *
     * Then you go to the note_freq[] array, find the index
     * of the entry for frequency 440.0 (let's say it's
     * j), then go to note_freq[j+4] to find the frequency
     * of the note you need to add.
     *
     * NOTE: If the value of  j+semitones  is < 0 or > 99
     *       then there is no note with the frequency you
     *       want. In that case you don't insert a new
     *       note.
     *
     * You then add a new note with that frequency at
     * bar=10 (same bar!)
     * index=.25 + .1  (the original index plus the time_shift)
     *
     * NOTE: If the resulting index is less than 0, or >= 1,
     *       then you DO NOT insert the new note.
     *
     * This function is crunchy - and if you don't think it
     * through before you start implementing it you'll run into
     * all kinds of trouble.
     *
     * This is the problem solving exercise for A2, so don't
     * look for people on Piazza to give you answers, or tell you
     * what to do, or verify you're doing the right thing.
     *
     * It's up to you how to solve this, and if you want an opinion,
     * you can come to visit us during office hours!
     *
     * Expected result: The BST will have about twice as many notes
     *   as before, and the new notes are shifted in pitch and
     *   in time as specified by the parameters.
     */

     harmonizer(root,root,semitones,time_shift);
     return root;
}
