
#include "IsraeliQueue.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ZERO 0

// Structs //

typedef struct Person_t{
    void* m_item;
    int m_numOfFriends;
    int m_numOfRivals;
    struct Person_t* m_next;
}Person;

struct IsraeliQueue_t{
    int m_friendPar;
    int m_rivalPar;
    FriendshipFunction* m_friendshipArr;
    ComparisonFunction* m_comparisonPtr;
    Person* m_first;
};

// ------- //



// Person functions //

void destroyPerson(Person* p){
    free(p);
}

void destroyAllPersons(Person* p){
    while(p){
        Person* toDelete = p;
        p=p->m_next;
        destroyPerson(toDelete);
    }
    /*
    Person* temp = p -> m_next;
    while(p){
        destroyPerson(p);
        p = temp;
        temp = temp -> m_next;
    }
     */
}

Person* createPerson(){
    Person* new = malloc(sizeof(Person));
    new -> m_item = NULL;
    new -> m_numOfFriends = 0;
    new -> m_numOfRivals = 0;
    new -> m_next = NULL;
    return new;
}

// Enqueues p at the end of the queue
Person* enqueuePerson(Person* first, Person* p){
    if(!first)
    {
        return p;
    }
    Person* temp = first;
    while(first -> m_next)
        first = first ->m_next;
    first -> m_next = p;
    return temp;
}

// Clones everything except m_next which is set to NULL!
Person* clonePerson(Person* p){
    Person* new = createPerson();
    new -> m_item = p -> m_item;
    new -> m_numOfFriends = p -> m_numOfFriends;
    new -> m_numOfRivals = p -> m_numOfRivals;
    new -> m_next = NULL;
    return new;
}

Person* clonePersons(Person* p){
    // if p is NULL, return NULL.
    if(!p)
    {
        return NULL;
    }

    // New Person to be returned.
    Person* new = clonePerson(p);// First person in new.
    p = p ->m_next;

    while(p){
        new = enqueuePerson(new, clonePerson(p));
        p = p -> m_next;
    }
    return new;
}

// --------------- //



// Misc. functions //


FriendshipFunction* cloneFriendshipArr(FriendshipFunction* friendshipArr){

    // Determine size of original NULL terminated array. If size is 0, return NULL.
    int size = 0;
    while(friendshipArr[size] != NULL)
    {
        size++;
    }
    if(size == 0)
    {
        return NULL;
    }
    // Malloc memory according to size. Check if successful.
    FriendshipFunction* clone = malloc(sizeof(FriendshipFunction) * size);
    if(clone == NULL)
    {
        return NULL;
    }
    // Populate new array.
    size--; // Begin from last entry of original.
    while(size >= 0)
    {
        clone[size] = friendshipArr[size];
        size--;
    }

    return clone;
}

void destroyFriendshipArr(FriendshipFunction* friendshipArr){
    free(friendshipArr);
}

bool areFriends(IsraeliQueue q, void* item, Person* p){

    // Param check.
    assert(q && item && p);

    int friendPar = q -> m_friendPar;

    int i = 0; // To go over every friendship function.

    // Call each function with these param, compare return value with queue param.
    while((q -> m_friendshipArr[i]))
    {
        if((q -> m_friendshipArr[i])(item, p->m_item) > friendPar)
        {
            return true;
        }

        i++;
    }
    // No function found items to be friends.
    return false;
}

bool canLetMeIn(IsraeliQueue q, void* item, Person* p){

    // Param check.
    assert(q && item && p);


    // Already let too many friends cut the queue.
    if(p -> m_numOfFriends >= FRIEND_QUOTA)
    {
        return false;
    }
    return(areFriends(q, item, p));
}

double friendshipAvg(IsraeliQueue q, void* item, Person* p){

    // Param check.
    assert(q && item && p);

    double sum = 0;

    int i = 0; // To go over every friendship function.

    // Call each function with these param.
    while((q -> m_friendshipArr[i]))
    {
        sum += (q -> m_friendshipArr[i])(item, p-> m_item );
        i++;
    }

    return sum/((double)(i));
}

bool canBlockMe(IsraeliQueue q, void* item, Person* p){

    // Param check.
    assert(q && item && p);
    // Already blocked too many rivals.
    if(p -> m_numOfRivals >= RIVAL_QUOTA)
    {
        return false;
    }
    // Are actually friends.
    if(areFriends(q, item, p))
    {
        return false;
    }
    // Now, if the average of the friendship values is lower than the param, p can block item.
    int rivalPar = q -> m_rivalPar;
    if(friendshipAvg(q, item , p) < rivalPar)
    {
        return true;
    }
    return false;
}

bool queueIsEmpty(IsraeliQueue q){
    if(!q)
    {
        return true;
    }
    return(q -> m_first == NULL);
}

bool queueArrIsEmpty(IsraeliQueue* qArr){

    //Param check.
    if(!qArr)
    {
        return true;
    }
    int i = 0; // Go over all of qArr
    while(qArr[i]){
        if(!queueIsEmpty(qArr[i]))
        {
            return false;
        }
        i++;
    }
    return true;
}

// Returns upper whole value of arr's average friendshipPars.
int friendshipAverage(IsraeliQueue* arr){
    int count = 0;
    int sum = 0;
    int i=0;
    while(arr[i]){
        sum += arr[i] -> m_friendPar;
        count++;
        i++;
    }
    if(sum % count == 0)
    {
        return sum / count;
    }
    return ((int)(((double)sum) / count)) + 1;
}

// Returns upper whole value of arr's geometric average of rivalPar.
int rivalAverage(IsraeliQueue* arr){
    int count = 0;
    int prod = 1;
    int i=0;
    while(arr[i]){
        prod *= arr[i] -> m_rivalPar;
        count++;
        i++;
    }
    int attempt = 1;
    bool negative = false;
    if(prod == 0)
        return 0;
    if(prod < 0){
        prod*= -1;
    }
    for (int i = 1; i <= prod; i++){
        for(int j = 1; j <= count; j++){
            attempt *= i;
        }
        if(attempt >= prod){
            if(negative)
                return -i;
            else
                return i;
        }
        attempt = 1;
    }
    return -1;
}


// ----- //

// IsraeliQueue functions //

// Creates a new Israeli Queue, with NULL m_first Person.
IsraeliQueue IsraeliQueueCreate(FriendshipFunction* friendshipArr, ComparisonFunction comp, int friendTh, int rivalryTh){

    // Memory allocation and success check.
    IsraeliQueue newQueue = malloc(sizeof(*newQueue));
    if(!newQueue){
        return NULL;
    }

    // Field values set.
    newQueue -> m_friendshipArr = friendshipArr;
    newQueue -> m_comparisonPtr = &comp;
    newQueue -> m_friendPar = friendTh;
    newQueue -> m_rivalPar = rivalryTh;
    newQueue -> m_first = NULL;
    return newQueue;
}

// Returns newly allocated IsraeliQueue with distinct copies of original fields.
IsraeliQueue IsraeliQueueClone(IsraeliQueue q){

    //If q is NULL return NULL.
    if(!q)
    {
        return NULL;
    }
    //Create new values to be used by create.
    FriendshipFunction* newArr = cloneFriendshipArr(q -> m_friendshipArr);
    ComparisonFunction newComp = *(q -> m_comparisonPtr);
    int newFriendPar =  q -> m_friendPar;
    int newRivalPar = q -> m_rivalPar;

    //Create according to new values. Check success.
    IsraeliQueue clone = IsraeliQueueCreate(newArr, newComp, newFriendPar, newRivalPar);
    if(!clone){
        //free(newComp);
        free(newArr);
        free(clone);
        return NULL;
    }

    //Deal with Person objects.
    clone -> m_first = clonePersons(q -> m_first);
    return clone;
}

// Destroys all objects inside IsraeliQueue.
void IsraeliQueueDestroy(IsraeliQueue q){

    // If q is NULL, return.
    if(!q)
    {
        return;
    }

    if(q -> m_first)
    {
        destroyAllPersons(q -> m_first);
    }
    free(q->m_friendshipArr);
    free(q);
    return;
}

// Enqueues item in queue according to rules found in pdf.
IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void * item){

    //If q or item are NULL, return BAD_PARAMS
    if(!q || !item){
        return ISRAELIQUEUE_BAD_PARAM;
    }

    Person* p = q -> m_first;
    Person* friend = NULL;
    bool onFriend = false;
    bool stopWhile = false;
    Person* itemPerson = createPerson();
    itemPerson -> m_item = item;
    if(!p){
        q -> m_first = itemPerson;
        return ISRAELIQUEUE_SUCCESS;
    }
    while(!stopWhile){

        if(!p -> m_next){
            stopWhile = true;
        }

        // If we are not in scope of a friend, check if p is a friend.
        if(!onFriend){
            if(canLetMeIn(q, item, p)){
                onFriend = true;
                friend = p;
            }
        }

            // If we are in scope of a friend, check if p can block us.
        else{
            if(canBlockMe(q, item, p)){
                p -> m_numOfRivals++;
                onFriend = false;
            }
        }

        // We have reached the end of the queue, and have a friend who can let us in uninterrupted.
        if(!(p -> m_next) && onFriend){
            friend -> m_numOfFriends++;

            Person* temp = friend -> m_next;
            friend -> m_next = itemPerson;
            itemPerson -> m_next = temp;

            return ISRAELIQUEUE_SUCCESS;
        }
        if(!stopWhile) {
            p = p->m_next; // Take a look at the next person in line.
        }
    }

    // If queue could not be cut, enqueue at the end.
    p -> m_next = itemPerson;
    return ISRAELIQUEUE_SUCCESS;
}

// Increases size of friendshipArr and places f at the end. Arr still ends with NULL.
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue q, FriendshipFunction f){

    // Determine size of original array.
    int size = 0;
    while(q -> m_friendshipArr[size])
        size++;

    // Realloc memory for a new array plus new function plus NULL termination.
    q -> m_friendshipArr = realloc(q -> m_friendshipArr, sizeof(FriendshipFunction) * (size + 2));
    if(!q)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    // Assigns f and NULL to end of array.
    q -> m_friendshipArr[size] = f;
    q -> m_friendshipArr[size + 1] = NULL;
    return ISRAELIQUEUE_SUCCESS;
}

// Sets new Threshold.
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int threshold){
    if(!q)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    q -> m_friendPar = threshold;
    return ISRAELIQUEUE_SUCCESS;
}

// Sets new Threshold.
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int threshold){
    if(!q)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    q -> m_rivalPar = threshold;
    return ISRAELIQUEUE_SUCCESS;
}

// Goes over all Persons until NULL is found. if NULL returns 0;
int IsraeliQueueSize(IsraeliQueue q){
    if(!q)
    {
        return 0;
    }
    int size = 0;
    Person* temp = q -> m_first;
    while(temp){
        size++;
        temp = temp -> m_next;
    }
    return size;
}

// q's queue is decapitated. m_first will be returned.
void* IsraeliQueueDequeue(IsraeliQueue q){

    // Param check.
    if(!q)
    {
        return NULL;
    }
    // Check Persons exist
    if(!(q -> m_first))
    {
        return NULL;
    }
    Person* toDelete = q -> m_first ;
    void* tempItem = q -> m_first -> m_item;
    q->m_first=q->m_first->m_next;
    if(!toDelete)
    {
        return NULL;
    }
    destroyPerson(toDelete);
    return tempItem;
}

// If either param is NULL, return false.
bool IsraeliQueueContains(IsraeliQueue q, void * item){
    if(!q || !item)
    {
        return false;
    }
    Person* p = q -> m_first;
    while(p){
        if((*(q -> m_comparisonPtr))(p, item) == 1)
        {
            return true;
        }
        p = p -> m_next;
    }
    return false;
}

// Improves position according to pdf.
IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue q){

    //Param check.
    if(!q)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    if(!(q -> m_first))
    {
        return ISRAELIQUEUE_SUCCESS;
    }
    // We use this copy to keep track of the order in which we need to search for the next person.
    IsraeliQueue original = IsraeliQueueClone(q);
    int originalSize = IsraeliQueueSize(original);
    int cursor = 1; // cursor over original.
    Person* pCursor = original -> m_first;

    Person* pCurrent = q -> m_first;
    Person* pBefore = q -> m_first;
    Person* temp = NULL;
    ComparisonFunction comp = *(q -> m_comparisonPtr);

    while(originalSize > 0){
        // Fetches Person to be looked for in q.
        while(cursor < originalSize){
            cursor++;
            pCursor = pCursor -> m_next;
        }
        cursor = 0;
        originalSize--;

        // Locates Person in q.
        while(comp(pCursor, pCurrent) != 1){ // Perhaps a different value here. Who said comp will always return 1 when true?
            pCurrent = pCurrent -> m_next;
        }

        // If person is first in queue
        if(comp(q -> m_first, pCurrent) == 1){
            temp = IsraeliQueueDequeue(q);
            IsraeliQueueEnqueue(q, temp);
        }

        else{

            // Find Person right before pCurrent.
            while(comp(pBefore -> m_next, pCurrent) != 1){
                pBefore = pBefore -> m_next;
            }
            pBefore -> m_next = pCurrent -> m_next;
            pCurrent -> m_next = NULL;
            IsraeliQueueEnqueue(q, pCurrent);
        }
    }
    IsraeliQueueDestroy(original);
    return ISRAELIQUEUE_SUCCESS;
}

// Merges queue according to pdf.
IsraeliQueue IsraeliQueueMerge(IsraeliQueue* qArr,ComparisonFunction comp){

    // Param check.
    if(!qArr)
    {
        return NULL;
    }
    int newFriendshipPar = friendshipAverage(qArr);
    int newRivalryPar = rivalAverage(qArr);
    FriendshipFunction* friendshipArray = malloc(sizeof(FriendshipFunction));
    friendshipArray[0] = NULL;
    IsraeliQueue new = IsraeliQueueCreate(friendshipArray, comp, newFriendshipPar, newRivalryPar);
    // Merges all friendship arrays.
    int i = 0;
    int j = 0;
    while(qArr[i]){
        while(qArr[i] -> m_friendshipArr[j]){

            IsraeliQueueAddFriendshipMeasure(new, qArr[i] -> m_friendshipArr[j]);
            j++;
        }
        j=0;
        i++;
    }
    int k = 0;

    // Keep going until the entire array no longer has any Persons in it.
    while(!queueArrIsEmpty(qArr)){

        // If gone too far, go back to first queue and look again.
        if(!qArr[k]){
            k = 0;
            continue;
        }

        // If current queue has a Person in it, dequeue and enqueue into new.
        if(!queueIsEmpty(qArr[k])){
            Person* temp = IsraeliQueueDequeue(qArr[k]);
            temp->m_numOfRivals = 0;
            temp->m_numOfFriends = 0;
            IsraeliQueueEnqueue(new,temp);
        }

        k++;
    }



    return new;
}

// ------------------------- //






/*
IsraeliQueue singleClone(IsraeliQueue q){
    IsraeliQueue result = IsraeliQueueCreate(q -> m_friendshipPtr, *(q -> m_comparisonPtr), q -> m_friendPar, q -> m_rivalPar);
    result -> m_student ->  m_numOfFriends = q -> m_student ->  m_numOfFriends;
    result -> m_student -> m_numOfRivals = q -> m_student -> m_numOfRivals;
    result -> m_next = q -> m_next;
    return result;
}

IsraeliQueue IsraeliQueueClone(IsraeliQueue q)
{
    if(!q)
    {
        return NULL;
    }
    IsraeliQueue newQ = IsraeliQueueCreate(q->m_friendshipPtr,*(q->m_comparisonPtr),q->m_friendPar,q->m_rivalPar);
    IsraeliQueue head = newQ;
    while(q -> m_next){
        head -> m_next = malloc(sizeof (head -> m_next));
        memcpy(head->m_next,q->m_next,sizeof (*q->m_next)); //head=heat->m_next ;
        head = head -> m_next;
        q = q -> m_next;
    }
    return newQ;

}

void IsraeliQueueDestroy(IsraeliQueue head)
{
    while(head)
    {
        IsraeliQueue toDelete=head;
        head=head-> m_next;
        free(toDelete);
    }
}

IsraeliQueueError IsraeliQueueEnqueueNoChange(IsraeliQueue head, void * item, bool change){
    IsraeliQueue q = (IsraeliQueue)(item);
    if(!head)
    {
        //check with some one what happends if the head is NULL
    }
    while(head){
        //this if runs until we find a friend
        if(!checkIfFriend(q,head)){//all good !
            head = head -> m_next;
            continue;
        }
        IsraeliQueue tempFriend = head;
        while(head){
            if(checkIfRival(head, q)){
                if(change) {
                    head -> m_student -> m_numOfRivals++;
                }
                head = head -> m_next;
                break;
            }
            head = head -> m_next;
            if(head == NULL){
                if(change) {
                    tempFriend -> m_student -> m_numOfFriends++;
                }
                IsraeliQueue temp = tempFriend -> m_next;
                tempFriend -> m_next = q;
                q -> m_next = temp;
                return ISRAELIQUEUE_SUCCESS;
            }
        }
    }
    head = q;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue head, void* item)
{

    return IsraeliQueueEnqueueNoChange(head, item, true);

}
//should talk about it with noam
//should allocate my own

FriendshipFunction* createFriendshipArray(int size){
    if(size <= 0)
        return NULL;
    FriendshipFunction* temp=malloc(sizeof(FriendshipFunction)*size);
    return temp;

}

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue q, FriendshipFunction friend)
{
    if(!q || !friend)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }

    FriendshipFunction* new = createFriendshipArray(q -> m_sizeOfFriendshipArray + 1);
    int i=0;
    for(; i < q -> m_sizeOfFriendshipArray; i++)
        new[i] = q -> m_friendshipPtr[i];
    new[i] = friend;
    free(q -> m_friendshipPtr);
    q -> m_friendshipPtr = new;
    q -> m_sizeOfFriendshipArray++;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int friendshipTh){
    while(q){
        q -> m_friendPar = friendshipTh;
        q = q -> m_next;
    }
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int rivalryTh){
    while(q){
        q -> m_friendPar = rivalryTh;
        q = q -> m_next;
    }
    return ISRAELIQUEUE_SUCCESS;
}

int IsraeliQueueSize(IsraeliQueue q){
    if(!q)
    {
        return ZERO;
    }
    int count = 0;
    while(q){
        count++;
        q = q -> m_next;
    }
    return count;
}

void* IsraeliQueueDequeue(IsraeliQueue q){
    if(!q) {
        return NULL;
    }
    IsraeliQueue result  = singleClone(q);
    Dequeue(q);
    return result;
}

bool IsraeliQueueContains(IsraeliQueue q, void* item){
    //HOW DO WE USE ITEM???????????!!!!!
    IsraeliQueue qItem = (IsraeliQueue)(item);
    if(!q || !item)
        return false;
    while(q){
        //here i changed the if beacuse we dont need to check if there next is the same. we dont know if they are in the same line
        if(qItem -> m_student -> m_numOfFriends == q -> m_student -> m_numOfFriends &&
           qItem -> m_student -> m_numOfRivals == q -> m_student -> m_numOfRivals &&
           qItem -> m_friendPar == q -> m_friendPar &&
           qItem -> m_rivalPar == q -> m_rivalPar &&
           qItem -> m_friendshipPtr == q -> m_friendshipPtr &&
           qItem -> m_comparisonPtr == q -> m_comparisonPtr )
            return true;
        q = q -> m_next;
    }
    return false;
}

IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue q){
    IsraeliQueue original = IsraeliQueueClone(q);
    IsraeliQueue head = q;
    IsraeliQueue temp = original;
    IsraeliQueue tempInsert;
    int size = IsraeliQueueSize(original);
    int iterations = size;
    while(iterations > 0){
        while(size > 0){
            temp = temp -> m_next;
            size--;
        }
        while(!areSame(head, temp)){
            head = head -> m_next;
        }
        tempInsert = IsraeliQueueDequeue(head);
        IsraeliQueueEnqueueNoChange(q, tempInsert, false);
        head = q;
        temp = original;
        iterations--;
        size = iterations;
    }
    return ISRAELIQUEUE_SUCCESS;
}

bool arrayExhausted(int arr [], int n){
    for(int i = 0; i < n; i++)
        if(arr[i] > 0)
            return false;
    return true;
}

int friendshipAverage(IsraeliQueue* arr){
    int count = 0;
    int sum = 0;
    while(arr){
        sum += (*arr) -> m_friendPar;
        count++;
        arr++;
    }
    if(sum % count == 0)
        return sum / count;
    return ((int)(((double)sum) / count)) + 1;
}

int rivalAverage(IsraeliQueue* arr){
    int count = 0;
    int prod = 1;
    while(arr){
        prod *= (*arr) -> m_rivalPar;
        count++;
        arr++;
    }
    int attempt = 1;
    bool negative = false;
    if(prod == 0)
        return 0;
    if(prod < 0){
        negative = true;
        prod *= -1;
        if(count % 2 == 0)
            return -1;
    }
    for (int i = 1; i < prod; i++){
        for(int j = 1; j <= count; j++)
            attempt *= i;
        if(attempt >= prod){
            if(negative)
                return -i;
            else
                return i;
        }
        attempt = 1;
    }
    return -1;
}



FriendshipFunction* mergeFriendships(IsraeliQueue* arr){
    int size = 0;
    int index = 0;
    FriendshipFunction* temp;
    while(arr[index]){
        temp = arr[index] -> m_friendshipPtr;
        while(*temp){
            size++;
            temp++;
        }
        index++;
    }
    if(size == ZERO)
        return NULL;
    FriendshipFunction* result = malloc((size+1) * sizeof(result));
    int count = 0;
    index = 0;
    while(count < size){
        temp = arr[index] -> m_friendshipPtr;
        while(*temp){
            result[count] = *temp;
            count++;
            temp++;
        }
        index++;
    }
    result[count] = NULL;
    return result;
}
/i put this in remark since we use this only at hack enollment
void IsraeliQueueEndOfLine(IsraeliQueue q, IsraeliQueue enqueue){
    if(!q){
        *q = *enqueue;
        return;
    }
    while(q -> m_next){
        q = q -> m_next;
    }
    q -> m_next = enqueue;
}

IsraeliQueue IsraeliQueueMerge(IsraeliQueue* arr,ComparisonFunction compareFunction){

    // Parameter check.

    if(!arr)
        return NULL;

    // Establishes parameters for new queue.

    int friendshipPar = friendshipAverage(arr);
    int rivalryPar = rivalAverage(arr);
    FriendshipFunction* friends = mergeFriendships(arr);

    IsraeliQueue* arrTemp = arr; //Look through arr without losing original pointer.

    // Establishes size of arr.

    int histCount = 0;
    while(arrTemp){
        histCount++;
        arrTemp++;
    }

    arrTemp = arr;

    // Creates int histogram, indicating how many entries in each queue by order of queues.

    int* hist = malloc(histCount*sizeof(int));
    for(int i = 0; i < histCount; i++){
        hist[i] = IsraeliQueueSize(*arrTemp);
        arrTemp++;
    }

    arrTemp = arr;

    // Frees memory of no longer needed friendship functions array of each queue.

    while(arrTemp){
        free((*arrTemp) -> m_friendshipPtr);
        arrTemp++;
    }

    // bigQ is queue to be returned.

    IsraeliQueue bigQ = malloc(sizeof(bigQ));
    bigQ = NULL;
    IsraeliQueue temp;

    // while there are still entries to be enqueued (while i is smaller than index of last queue(skip empty queues,
    // enqueue otherwise) i++)

    int i = 0;
    while(!arrayExhausted(hist, histCount)){
        while(i < histCount){
            if(hist[i] == 0){
                i++;
                break;
            }
            if(bigQ == NULL){
                bigQ = IsraeliQueueDequeue((arr[i]));
                bigQ -> m_student ->  m_numOfFriends = 0;
                bigQ -> m_student -> m_numOfRivals = 0;
                bigQ -> m_friendPar = friendshipPar;
                bigQ -> m_rivalPar = rivalryPar;
                bigQ -> m_friendshipPtr = friends;
                bigQ -> m_comparisonPtr = &compareFunction;
                bigQ -> m_next = NULL;
            }
            else{
                temp = IsraeliQueueDequeue((arr[i]));
                temp -> m_student -> m_numOfFriends = 0;
                temp -> m_student -> m_numOfRivals = 0;
                temp -> m_friendPar = friendshipPar;
                temp -> m_rivalPar = rivalryPar;
                temp -> m_friendshipPtr = friends;
                temp -> m_comparisonPtr = &compareFunction;
                temp -> m_next = NULL;
                IsraeliQueueEnqueue(bigQ, temp);
            }
            hist[i]--;
            i++;
        }
        i = 0;
    }
    return bigQ;
}

bool checkIfFriend(IsraeliQueue hacker,IsraeliQueue student)
{
    if(student -> m_student -> m_numOfFriends >= FRIEND_QUOTA)
    {
        return false;
    }

    while(student -> m_friendshipPtr) {

        if ((*student -> m_friendshipPtr)(hacker,student) >student-> m_friendPar) {//all good
            return true;
        }
        student -> m_friendshipPtr++;
    }
    return false;
}

bool checkIfRival(IsraeliQueue head,IsraeliQueue item)
{
    if(head -> m_student -> m_numOfRivals >= RIVAL_QUOTA)
    {
        return false;
    }
    if(checkIfFriend(head, item))
    {
        return false;
    }
    int sum=0,cnt=0;
    while(head -> m_friendshipPtr)
    {
        cnt++;
        sum+=(*head -> m_friendshipPtr)(head, item);
        head -> m_friendshipPtr++;
    }
    if((sum/cnt)<(head->m_rivalPar))
    {
        return true;
    }
    return false;
}

void Dequeue(IsraeliQueue q){
    while(q -> m_next){
        q -> m_student -> m_numOfFriends = q -> m_next ->m_student ->  m_numOfFriends;
        q -> m_student -> m_numOfRivals = q -> m_next -> m_student -> m_numOfRivals;
        q -> m_friendPar = q -> m_next -> m_friendPar;
        q -> m_rivalPar = q -> m_next -> m_rivalPar;
        q = q-> m_next;
    }
    free(q);
}

bool areSame(IsraeliQueue q1, IsraeliQueue q2){
    return (q1 ->m_student ->  m_numOfFriends == q2 ->m_student ->  m_numOfFriends && q1 -> m_student -> m_numOfRivals == q2 -> m_student -> m_numOfRivals && q1 -> m_friendPar == q2 -> m_friendPar && q1 -> m_rivalPar == q2 -> m_rivalPar);
}
*/