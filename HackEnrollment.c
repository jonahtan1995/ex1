
#include "HackEnrollment.h"

// Strucs //




typedef struct Student{
    int m_studentId;
    int m_totalCredits;
    int m_gpa;
    bool m_hacker;
    char* m_name;
    char* m_surname;
    char* m_city;
    char* m_department;
    int* m_courses;
    int m_numOfCourses;
    int* m_friends;
    int m_numOfFriends;
    int* m_rivals;
    int m_numOfRivals;
    int m_missedCourses;
}Student;

typedef struct Course{
    int m_courseNum;
    int m_size;
    IsraeliQueue m_queue;
}Course;

struct EnrollmentSystem_t{
    Student** m_students;
    int m_studentsSize;
    Course** m_courses;
    int m_coursesSize;
    bool m_ignoreCase;
};

// ----- //



// Comparison Function //

int studentCompare(void* st1, void* st2){

    // Param check.
    if(!st1 || ! st2)
    {
        return 0;
    }

    // If both point to same object, students are the same student.

    if(st1 == st2)
    {
        return 1;
    }
    return 0;
}

// ------------------- //




// Misc. funcs //




int maxString(int n1,int n2)
{
    if(n1>n2)
    {
        return n1;
    }
    return n2;

}

//If num is ID of one of the students, return true
/*bool isId(int num, Student* students[], int arrSize){
    for(int i = 0; i < arrSize; i++){
        if(students[i] -> m_studentId == num){
            return true;
        }
    }
    return false;
}
*/

int charToNum(char c){
    if(!(c >= '0' && c <= '9'))
    {
        return -1;
    }
    return (c - '0');
}

int longestLine(FILE* fp){
    char c;
    int count = 0;
    int max = -1;
    while((c = fgetc(fp)) != EOF){
        if(c != '\n')
        {
            count++;
        }
        else{
            count++;
            if(count > max)
            {
                max = count;
            }

            count = 0;
        }
    }
    rewind(fp);
    return max;
}

int getLongestString(FILE* fp)
{
    int longest=0;
    int cnt=0;
    int curr= fgetc(fp);
    while (curr!=EOF)
    {
        while(curr!='\n') { //ATTENTION!: If file does not end with \n, this will not work! must add || curr != EOF.
            if (curr == ' ') { //PDF specifically stated file will end with \n, so this was left out on purpose.
                cnt = 0;
                curr = fgetc(fp);
                continue;
            } else {
                cnt++;
                curr = fgetc(fp);
                if (curr == ' ') {
                    longest = maxString(longest, cnt);
                }
            }
        }
        longest = maxString(longest, cnt);
        cnt=0;
        curr = fgetc(fp);
    }
    rewind(fp);
    return longest;
}

FriendshipFunction* hackerFunctions(bool ignoreCase);


int getValue(char* c, bool ignoreCase){
    if(!((*c >= 'a' && *c <= 'z')||(*c >= 'A' && *c <= 'Z')))
        return 0;
    if(ignoreCase){
        if(*c >= 'a' && *c <= 'z')
            return *c;
        else
            return ('a'+ *c - 'A');
    }
    else
        return *c;
}

bool charIsNum(char c){
    return (c >= '0' && c <= '9');
}

bool charIsNewLine(char *c){
    return (*c == '\n');
}

char* strCpyVal(char* source, char* dest){
    if(!*source)
    {
        return NULL ;
    }
    int size = 0;
    char* cursor = source;
    while(*cursor){
        size++;
        cursor++;
    }
    cursor = source;
    char* new = malloc(sizeof(char)*(size + 1));
    char* temp = new;
    int i = 0;
    while(i < size){
        *temp = *cursor;
        i++;
        temp++;
        cursor++;
    }
    *temp = '\0';
    return new;
}

int numOfHackerField(FILE* fp){
    if (!fp){
        fclose(fp);
        return 0;
    }
    char curr= fgetc(fp);
    int numOfField = 0;
    bool onField = false;
    while((curr = fgetc(fp)) != '\n'){
        if(curr >='0' && curr<='9'){
            if(onField){
                continue;
            }
            else{
                numOfField++;
                onField = true;
            }
        }
        else{
            if(onField)
                onField = false;
        }
    }
    return numOfField;
}

int* parseArray(int arr[], int* size, char* line){
    int j = 0;
    bool stopWhile = false;
    bool onNum = false;
    long long tempNum=0;
    while(!stopWhile){
        if(*line == '\n')
        {
            stopWhile = true;
        }
        if(-1 != charToNum(*line)){
            onNum = true;
            tempNum += charToNum(*line);
            tempNum *=10;
            line++;
        }
        else{
            line++;
            if(!onNum)
            {
                continue;
            }
            else{
                onNum = false;
                tempNum /= 10;
                j++;
                arr = realloc(arr, j * sizeof(int));
                arr[j - 1] = tempNum;
                tempNum = 0;
            }
        }
    }
    *size = j;
    return arr;
}

void inputHackersFromFileToArray(Student* students[], int arrSize, FILE* fp){
    int tempStudentId;
    int maxLineLength = longestLine(fp);
    char* tempLine = malloc(sizeof(tempLine)*(maxLineLength+1));
    rewind(fp);
    char newLineEater = 0;
    while(fscanf(fp,"%d",&tempStudentId) != EOF){
        for(int i=0; i < arrSize; i++){
            if(students[i] -> m_studentId == tempStudentId){
                students[i] -> m_hacker=true;
                newLineEater = fgetc(fp);
                if(newLineEater){
                    newLineEater--; // newLineEater is a garbage variable!
                }

                // Deal with course array
                fgets(tempLine, maxLineLength + 1, fp);
                students[i]->m_courses = parseArray(students[i] -> m_courses, &(students[i] -> m_numOfCourses), tempLine);

                // Deal with friend array
                fgets(tempLine, maxLineLength + 1, fp);
                //students[i]->m_friends=NULL;
                students[i]->m_friends = parseArray(students[i] -> m_friends, &(students[i] -> m_numOfFriends), tempLine);

                // Deal with rival array
                fgets(tempLine, maxLineLength + 1, fp);
                students[i]->m_rivals=NULL;
                students[i]->m_rivals = parseArray(students[i] -> m_rivals, &(students[i] -> m_numOfRivals), tempLine);
            }
        }
    }
    free(tempLine);
    rewind(fp);
}

int getNumLines(FILE* fp){
    if (!fp) {
        return 0;
    }
    int size = 0;
    char line;
    while((line = fgetc(fp)) != EOF){
        if(line == '\n')
        {
            size++;
        }
    }
    return size;
    rewind(fp);
}

bool isHacker(Student* s){
    return s -> m_hacker;
}

// Friendship funcs
//jonah
int areFriendsByHacker(void* hackerQ,void* studentQ)
{
    Student* hacker=(Student*)hackerQ;
    Student* student=(Student*)studentQ;
    //here we check if both of them are not hacker
    if((hacker->m_hacker==false)&&(student->m_hacker==false))
    {
        return NEUTRAL;
    }
    //here we check if they are friends
    for(int i=0;i<hacker->m_numOfFriends;i++)
    {
        if(hacker->m_friends[i]==student->m_studentId)
        {
            return FRIEND;
        }
    }
    //here we check if they are enemies
    for(int i=0;i<hacker->m_numOfRivals;i++)
    {
        if(hacker->m_rivals[i]==student->m_studentId)
        {
            return RIVAL;
        }
    }
    return NEUTRAL;
}

int nameDelta(void* hackerQ, void* studentQ)
{
    Student* hacker=(Student*)hackerQ;
    Student* student=(Student*)studentQ;
    if((hacker->m_hacker==false)&&(student->m_hacker==false))
    {
        return NEUTRAL;
    }
    char* studentName = student->m_name;
    char* hackerName = hacker->m_name;
    int lenHacker=strlen(hacker->m_name);
    int lenStudent=strlen(student->m_name);
    int lenStudentSurname=strlen(student->m_surname);
    int lenHackerSurname=strlen(hacker->m_surname);
    int delta = 0;
    int tempStudent = 0;
    int tempHacker = 0;
    int longerName = lenStudent > lenHacker ? lenStudent : lenHacker;
    for(int i = 0; i < longerName; i++){
        if(*studentName != '\0'){
            tempStudent = getValue(studentName, false);
            studentName++;
        }
        if(*hackerName != '\0'){
            tempHacker = getValue(hackerName, false);
            hackerName++;
        }
        delta += tempStudent - tempHacker > 0 ? tempStudent - tempHacker : (-1) * (tempStudent - tempHacker);
        tempStudent = 0;
        tempHacker = 0;
    }
    studentName = student ->m_surname;
    hackerName = hacker ->m_surname;
    int longerSurname = lenStudentSurname > lenHackerSurname ? lenStudentSurname : lenHackerSurname;
    for(int i = 0; i < longerSurname; i++){
        if(*studentName != '\0'){
            tempStudent = getValue(studentName, false);
            studentName++;
        }
        if(*hackerName != '\0'){
            tempHacker = getValue(hackerName, false);
            hackerName++;
        }
        delta += tempStudent - tempHacker > 0 ? tempStudent - tempHacker : (-1) * (tempStudent - tempHacker);
        tempStudent = 0;
        tempHacker = 0;
    }
    return delta;
}

/*
int areFriends(void* hacker, void* student){
    IsraeliQueue hackerQ = (IsraeliQueue)hacker;
    IsraeliQueue studentQ = (IsraeliQueue)student;
    //If hacker has friends, look for student.
    if(hackerQ -> m_student->m_numOfFriends > 0)
        for(int i = 0; i < hackerQ ->m_student->m_numOfFriends; i++)
            if(hackerQ ->m_student-> m_friends[i] == studentQ->m_student->m_studentId) {
                return FRIENDS;
            }

    //If hacker has rivals, look for student.
    if(hackerQ ->m_student->m_numOfRivals > 0)
        for(int i = 0; i < hackerQ ->m_student-> m_numOfRivals; i++)
            if(hackerQ ->m_student-> m_rivals[i] == studentQ ->m_student->m_studentId) {
                return RIVALS;
            }
    // If student is neither a friend nor rival, return neutral.
    return NEUTRAL;
}
*/
int nameDeltaIgnoreCase(void* hackerQ, void* studentQ){
    Student* hacker=(Student*)hackerQ;
    Student* student=(Student*)studentQ;
    if((hacker->m_hacker==false)&&(student->m_hacker==false))
    {
        return NEUTRAL;
    }
    char* studentName = student ->m_name;
    char* hackerName = hacker ->m_name;
    int lenHacker=strlen(hacker->m_name);
    int lenStudent=strlen(student->m_name);
    int lenStudentSurname=strlen(student->m_surname);
    int lenHackerSurname=strlen(hacker->m_surname);
    int delta = 0;
    int tempStudent = 0;
    int tempHacker = 0;
    int longerName = lenStudent > lenHacker ? lenStudent : lenHacker;
    for(int i = 0; i < longerName; i++){
        if(*studentName != '\0'){
            tempStudent = getValue(studentName, true);
            studentName++;
        }
        if(*hackerName != '\0'){
            tempHacker = getValue(hackerName, true);
            hackerName++;
        }
        delta += tempStudent - tempHacker > 0 ? tempStudent - tempHacker : (-1) * (tempStudent - tempHacker);
        tempStudent = 0;
        tempHacker = 0;
    }
    studentName = student ->m_surname;
    hackerName = hacker ->m_surname;
    int longerSurname = lenStudentSurname > lenHackerSurname ? lenStudentSurname : lenHackerSurname;
    for(int i = 0; i < longerSurname; i++){
        if(*studentName != '\0'){
            tempStudent = getValue(studentName, true);
            studentName++;
        }
        if(*hackerName != '\0'){
            tempHacker = getValue(hackerName, true);
            hackerName++;
        }
        delta += tempStudent - tempHacker > 0 ? tempStudent - tempHacker : (-1) * (tempStudent - tempHacker);
        tempStudent = 0;
        tempHacker = 0;
    }
    return delta;
}
/*
int nameDelta(void* hackerQ, void* studentQ){
    IsraeliQueue hacker=(IsraeliQueue)hackerQ;
    IsraeliQueue student=(IsraeliQueue)studentQ;
    char* studentName = student->m_student ->m_name;
    char* hackerName = hacker->m_student ->m_name;
    int lenHacker=strlen(hacker->m_student->m_name);
    int lenStudent=strlen(student->m_student->m_name);
    int lenStudentSurname=strlen(student->m_student->m_surname);
    int lenHackerSurname=strlen(hacker->m_student->m_surname);
    int delta = 0;
    int tempStudent = 0;
    int tempHacker = 0;
    int longerName = lenStudent > lenHacker ? lenStudent : lenHacker;
    for(int i = 0; i < longerName; i++){
        if(*studentName != '\0'){
            tempStudent = getValue(studentName, false);
            studentName++;
        }
        if(*hackerName != '\0'){
            tempHacker = getValue(hackerName, false);
            hackerName++;
        }
        delta += tempStudent - tempHacker > 0 ? tempStudent - tempHacker : (-1) * (tempStudent - tempHacker);
        tempStudent = 0;
        tempHacker = 0;
    }
    studentName = student->m_student ->m_surname;
    hackerName = hacker ->m_student->m_surname;
    int longerSurname = lenStudentSurname > lenHackerSurname ? lenStudentSurname : lenHackerSurname;
    for(int i = 0; i < longerSurname; i++){
        if(*studentName != '\0'){
            tempStudent = getValue(studentName, false);
            studentName++;
        }
        if(*hackerName != '\0'){
            tempHacker = getValue(hackerName, false);
            hackerName++;
        }
        delta += tempStudent - tempHacker > 0 ? tempStudent - tempHacker : (-1) * (tempStudent - tempHacker);
        tempStudent = 0;
        tempHacker = 0;
    }
    return delta;
}
*/

int idDelta(void* hackerQ, void* studentQ){
    Student* hacker=(Student*)hackerQ;
    Student* student=(Student*)studentQ;
    if((hacker->m_hacker==false)&&(student->m_hacker==false))
    {
        return NEUTRAL;
    }
    int delta = (hacker ->m_studentId) - (student -> m_studentId);
    if (delta < 0)
    {
        return ((-1) * delta);
    }
    return delta;
}

FriendshipFunction* newFriendship(int length){
    if(!(length > -1))
    {
        return NULL;
    }
    FriendshipFunction* new=malloc(sizeof(FriendshipFunction)*length);
    return new;
}

FriendshipFunction* hackerFunctions(bool ignoreCase){
    //here we allocate the size of array of pointers to  FriendshipFunction
    FriendshipFunction* f = newFriendship(4);

    //here i did casting since the friendship function cant be friendshipFunction since there are returning int
    f[0] = (FriendshipFunction)&areFriendsByHacker;
    if(ignoreCase)
    {
        f[1] = (FriendshipFunction)&nameDeltaIgnoreCase;
    }
    else
    {
        f[1] = (FriendshipFunction)&nameDelta;
    }
    f[2] = (FriendshipFunction)&idDelta;
    f[3]=NULL;
    return f;
}
// Courses funcs

Course* createCourse(int courseNum,int size)
{
    Course* new = malloc(sizeof(Course));
    new -> m_courseNum = courseNum;
    new -> m_size = size;
    new -> m_queue = NULL;
    return new;
}

Course* duplicateSingle(Course* course){ // m_next set to NULL
    Course* new = malloc(sizeof(Course));
    new -> m_courseNum = course -> m_courseNum;
    new -> m_size = course -> m_size;
    new -> m_queue = NULL;
    return new;
}
/*
void courseEnqueue(Course* courses, Course* new){
    while(courses)
        courses = courses -> m_next;
    courses = new;
}
*/

FriendshipFunction* duplicateFriendshipArr(FriendshipFunction* f){
    int size = 0;
    while(f[size]){
        size++;
    }
    FriendshipFunction* new = malloc(sizeof(FriendshipFunction) * (size + 1));

    size = 0;
    while(f[size]){
        new[size] = f[size];
        size++;
    }
    new[size] = NULL;

    return new;
}

void inputCoursesFromFileToArray(EnrollmentSystem sys, Course* coursesArray[],FILE* courses)
{

    // Creates FriendshipFunction array and Comparison Function for the new queues.
    FriendshipFunction* f = hackerFunctions(sys -> m_ignoreCase);
    ComparisonFunction c = studentCompare;

    Course* head = malloc(sizeof(Course));
    int i=0;
    while(!(feof(courses))){
        int n = fscanf(courses,"%d %d",&(head -> m_courseNum), &(head -> m_size));
        if(n != 2)
        {
            break;
        }
        coursesArray[i] = duplicateSingle(head);
        FriendshipFunction* perCourse = duplicateFriendshipArr(f);
        coursesArray[i] -> m_queue = IsraeliQueueCreate(perCourse, c, FRIEND_PAR, RIVAL_PAR);
        i++;
    }
    free(f);
    rewind(courses);
    free(head);
}

void considerCase(EnrollmentSystem sys, bool ignoreCase, FILE* courses){
    sys->m_ignoreCase=ignoreCase;
    inputCoursesFromFileToArray(sys, sys -> m_courses, courses);
}

// Students funcs

//this jonah did

Student*createStudent(int longestString)
{// dont forget to release these mallocs
    Student* newStudent = malloc(sizeof(Student));
    newStudent->m_studentId=0;
    newStudent->m_totalCredits=0;
    newStudent->m_gpa=0;
    newStudent->m_name= malloc(sizeof(char)*longestString);
    newStudent->m_city=malloc(sizeof(char)*longestString );
    newStudent->m_surname=malloc(sizeof(char)*longestString );
    newStudent->m_department=malloc(sizeof(char)*longestString );
    newStudent->m_courses =NULL;
    newStudent->m_numOfCourses = 0;
    newStudent->m_friends = NULL;
    newStudent->m_numOfFriends = 0;
    newStudent->m_rivals = NULL;
    newStudent->m_numOfRivals = 0;
    newStudent->m_missedCourses = 0;
    return newStudent;
}

void destroyStudent(Student* student){
    if(student==NULL) {
        return;
    }
    free(student -> m_name);
    free(student -> m_surname);
    free(student -> m_city);
    free(student -> m_department);
    free(student->m_friends);
    free(student->m_courses);
    free(student->m_rivals);
    free(student);
}

void destroyAllStudents(Student* students[],int size){
    for(int i=0;i<size;i++) {
        destroyStudent(students[i]);
    }
    free(students);
}

//this free Israeli quque per course
void destroyCourse(Course* course) {
    if(!course)
    {
        return;
    }
    IsraeliQueueDestroy(course->m_queue);
    free(course);
}
//this destroy the whole array of courses
void destroyAllCourses(Course** courses,int size)
{
    for(int i=0;i<size;i++)
    {
        destroyCourse(courses[i]);
    }
    free(courses);
}
//this destroy Enrollment sys
void destroyEnrollment(EnrollmentSystem sys)
{
    destroyAllStudents(sys->m_students,sys->m_studentsSize);
    //free(sys->m_students);
    destroyAllCourses(sys->m_courses,sys->m_coursesSize);
    free(sys);

    //a problem can be not free the student** and courses** but there is no malloc there so i am not sure

}



/*Student* studentCreate(int longestString, int studentId, int totalCredits, int gpa, char* name, char* surname, char* city, char* department, Course* courses){
    Student* student = createStudent(longestString);
    student -> m_studentId = studentId;
    student -> m_totalCredits = totalCredits;
    student -> m_gpa = gpa;
    student -> m_name=NULL;
    student -> m_surname=NULL;
    student -> m_city=NULL;
    student -> m_department=NULL;
    student -> m_name = name;
    student -> m_surname = surname


    //strCpyVal(name, student -> m_name);
    //strCpyVal(surname, student -> m_surname);
    //strCpyVal(city, student -> m_city);
    //strCpyVal(department, student -> m_department);
    return student;
}
 */

Student* duplicateStudent(Student* student){
    Student* new = malloc(sizeof(Student));
    new->m_studentId=student->m_studentId;
    new->m_totalCredits=student->m_totalCredits;
    new->m_gpa=student->m_gpa;
    new->m_name=student->m_name;
    new->m_surname=student->m_surname;
    new->m_city=student->m_city;
    new->m_department=student->m_department;
    new->m_courses=NULL;
    new->m_friends=NULL;
    new->m_rivals=NULL;
    new->m_numOfCourses=0;
    new->m_numOfFriends=0;
    new->m_numOfRivals=0;
    return new;
}

// Get the position of a student in a queue. Counts from 0.
int getPositionOfIn(Student* tempHacker,IsraeliQueue curr){

    //Param check.
    if(!tempHacker || !curr){
        return -1;
    }

    IsraeliQueue queueCopy = IsraeliQueueClone(curr);
    Student* temp = NULL;
    int position = 0;
    while(IsraeliQueueSize(queueCopy) > 0){
        temp = IsraeliQueueDequeue(queueCopy);
        if(studentCompare(tempHacker, temp) == 1){
            // destroyStudent(temp);
            IsraeliQueueDestroy(queueCopy);
            return position;
        }
        position++;
    }
    IsraeliQueueDestroy(queueCopy);
    return -1;
}

/*int longestWord(FILE fp){
    bool onWord = false;
    int count = 0;
    int max = 0;
    char c = '\0';
    while((c = getc(fp)) != EOF){
        if(c == ' '){
            if(onWord){
                if
            }
        }
    }
}*/



void inputStudentsFromFileToArray(Student** studentsArray,FILE* students,int size)
{
    int longestString=getLongestString(students);
    int i=0;
    while ((!feof(students))) {
        if(i<size) {
            studentsArray[i] = createStudent(longestString+1);
            studentsArray[i] -> m_friends = NULL;
            int n = fscanf(students, "%d %d %d %s %s %s %s",
                           &(studentsArray[i]->m_studentId), &(studentsArray[i]->m_totalCredits),
                           &(studentsArray[i]->m_gpa), studentsArray[i]->m_name,
                           studentsArray[i]->m_surname, studentsArray[i]->m_city,
                           studentsArray[i]->m_department);
            if (n != 7) {
                break;
            }
            studentsArray[i]->m_hacker = false;
            studentsArray[i]->m_friends=NULL;
            i++;
        }
        else {
            return;
        }

    }

}


// Enrollment funcs
/*
EnrollmentSystem createEnrollmentWithFlag(FILE* students, FILE* courses, FILE* hackers, bool ignoreCase){

}
 */
char* convertInrToString(int Id)
{
    char* intToString=malloc(sizeof(char)*10);
    sprintf(intToString,"%d",Id);
    return intToString;
}

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers){//jonah check good
    EnrollmentSystem enrollmentSystem = malloc(sizeof(*enrollmentSystem));
    //check if malloc sucssed
    if(!enrollmentSystem){
        free(enrollmentSystem);
        return NULL;
    }
    int studentsSize = getNumLines(students);
    rewind(students);
    int coursesSize = getNumLines(courses);
    rewind(courses);
    Student** studentArray = malloc(sizeof(Student*)*studentsSize);
    Course** courseArray = malloc(sizeof(Course*)*coursesSize);
    inputStudentsFromFileToArray(studentArray, students, studentsSize);
    rewind(students);

    rewind(courses);
    inputHackersFromFileToArray(studentArray, studentsSize, hackers);
    rewind(hackers);
    enrollmentSystem -> m_students = studentArray;
    enrollmentSystem -> m_courses = courseArray;
    enrollmentSystem -> m_studentsSize = studentsSize;
    enrollmentSystem -> m_coursesSize = coursesSize;
    rewind(students);
    rewind(courses);
    rewind(hackers);
    return enrollmentSystem;
}

// Creates IsraeliQueues for each course, populating them according to fp.
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* fp){

    //check if either is NULL
    if(!fp||!sys)
    {
        return NULL;
    }

    int i = 0;
    int j = 0;
    int courseNum = 0;
    char curr=0;
    long long studentId = 0;
    bool onNum = false;
    bool stopWhile=false;
    bool wasHacker = false;

    // Reads all courses, until none left.
    while(fscanf(fp,"%d",&courseNum) != EOF){

        // Find index of course with courseNum.
        for(; i < sys->m_coursesSize; i++){
            if(sys->m_courses[i]->m_courseNum == courseNum){
                break;
            }
        }

        // Check that a course was actually found.
        if(i == sys->m_coursesSize)
        {
            return NULL;
        }

        // Create IsraeliQueue in course.
        //sys -> m_courses[i] -> m_queue = IsraeliQueueCreate(f, c, FRIEND_PAR, RIVAL_PAR);
        IsraeliQueue currentQueue = sys -> m_courses[i] -> m_queue;

        // Read IDs until new line
        while(!stopWhile){
            if ((curr = fgetc(fp)) == '\n')
            {
                stopWhile=true;
            }

            //go in when character is not a number
            if(!(charIsNum(curr)) && !onNum)
            {
                continue;
            }

            //Get student ID
            if(charIsNum(curr)){
                onNum = true;
                studentId *= 10;
                studentId += curr - '0';
                continue;
            }
            onNum = false;

            //Find student in array use J
            for(;j<sys->m_studentsSize;j++){
                if(sys->m_students[j]->m_studentId == studentId){
                    break;
                }
            }

            // Place student j, at the end of the queue for course i
            if(sys -> m_students[j] ->m_hacker){
                sys -> m_students[j] ->m_hacker = false;
                wasHacker = true;
            }
            IsraeliQueueEnqueue(currentQueue, sys -> m_students[j]);//possible problem

            if(wasHacker){
                wasHacker = false;
                sys -> m_students[j] ->m_hacker = true;
            }
            //Variable reset
            studentId = 0;
            j = 0;
        }
        stopWhile=false;
        i=0;
    }
    rewind(fp);
    return sys;
}

//
void hackEnrollment(EnrollmentSystem sys, FILE* out){

    // Param check.
    if(!sys){
        return;
    }

    Student* hacker;
    int l = 0;

    //Enqueues hackers and ends function if conditions were not met.
    for(int i = 0; i < sys -> m_studentsSize; i++){

        //check if student is hacker
        if(isHacker(sys->m_students[i])){
            hacker = sys->m_students[i];

            //Go over all of hacker's courses.
            for(int j = 0; j < hacker -> m_numOfCourses; j++){

                //here we find the correct course in the array courses
                for(; l < sys->m_coursesSize; l++){
                    if(sys->m_courses[l]->m_courseNum == hacker -> m_courses[j]){
                        break;
                    }
                }

                // Check that a course was actually found.
                if(l == sys->m_coursesSize){
                    return;
                }

                // Better looking variable and enqueue hacker.
                IsraeliQueue curr = sys->m_courses[l]->m_queue;
                IsraeliQueueEnqueue(curr,hacker);

                // Checks if conditions were not met and by whom.
                for(int k = 0; k < sys -> m_studentsSize; k++){
                    if(isHacker(sys -> m_students[k])){

                        // Better looking variable
                        Student* tempHacker = sys -> m_students[k];

                        // If hacker's position is greater than size
                        if(getPositionOfIn(tempHacker, curr) >= sys -> m_courses[l] -> m_size) {
                            tempHacker->m_missedCourses++;
                            if (tempHacker->m_numOfCourses == 1 || tempHacker->m_missedCourses == 2) {
                                fprintf(out, "Cannot satisfy constraints for %d\n", tempHacker->m_studentId);
                                return;
                            }
                        }
                    }
                }
                l = 0;
            }
        }
    }
    //Prints queues to out
    for(int i = 0; i < sys -> m_coursesSize; i++){
        Course* tempCourse = sys -> m_courses[i];
        IsraeliQueue tempQueue = tempCourse -> m_queue;

        if(IsraeliQueueSize(tempQueue) > 0) {
            char *currCourseNum = convertInrToString(tempCourse->m_courseNum);
            fprintf(out, "%s", currCourseNum);
            free(currCourseNum);
            while (IsraeliQueueSize(tempQueue) > 0) {
                Student *tempStudent = IsraeliQueueDequeue(tempQueue);
                char *currStudentId = convertInrToString(tempStudent->m_studentId);
                fprintf(out, " %s", currStudentId);
                //destroyStudent(tempStudent);
                free(currStudentId);
            }
            fprintf(out, "\n");
        }
    }
}