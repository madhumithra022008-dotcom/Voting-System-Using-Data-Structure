#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------- Candidate --------
struct Candidate {
    int id;
    char name[50];
    int votes;
};

// -------- Tree (Region) --------
struct Tree {
    char region[50];
    struct Candidate candidates[3];
    int candidateCount;
    struct Tree *left, *right;
};

// -------- Linked List (Voters) --------
struct Voter {
    char id[20];
    struct Voter* next;
};

struct Voter* voterHead = NULL;

// -------- Create Region --------
struct Tree* createRegion(char name[]) {
    struct Tree* node = (struct Tree*)malloc(sizeof(struct Tree));
    strcpy(node->region, name);

    node->candidateCount = 3;

    node->candidates[0] = (struct Candidate){1, "Alice", 0};
    node->candidates[1] = (struct Candidate){2, "Bob", 0};
    node->candidates[2] = (struct Candidate){3, "Charlie", 0};

    node->left = node->right = NULL;
    return node;
}

// -------- Check if Voter Already Voted --------
int hasVoted(char id[]) {
    struct Voter* temp = voterHead;

    while (temp != NULL) {
        if (strcmp(temp->id, id) == 0)
            return 1;
        temp = temp->next;
    }
    return 0;
}

// -------- Add Voter --------
void addVoter(char id[]) {
    struct Voter* newNode = (struct Voter*)malloc(sizeof(struct Voter));
    strcpy(newNode->id, id);
    newNode->next = voterHead;
    voterHead = newNode;
}

// -------- Voting (Tree Traversal) --------
void voteInRegion(struct Tree* root, char regionName[]) {
    if (root == NULL) return;

    if (strcmp(root->region, regionName) == 0) {
        char voter_id[20];

        printf("\nEnter Voter ID: ");
        scanf("%s", voter_id);

        // Check duplicate vote
        if (hasVoted(voter_id)) {
            printf("? You have already voted!\n");
            return;
        }

        int choice;

        printf("\nVoting in %s\n", root->region);
        int i;
        for (i = 0; i < root->candidateCount; i++) {
            printf("%d. %s\n", root->candidates[i].id,
                   root->candidates[i].name);
        }

        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice >= 1 && choice <= root->candidateCount) {
            root->candidates[choice - 1].votes++;
            addVoter(voter_id);  // store voter
            printf("? Vote casted successfully\n");
        } else {
            printf("? Invalid choice\n");
        }
        return;
    }

    // Traverse tree
    voteInRegion(root->left, regionName);
    voteInRegion(root->right, regionName);
}

// -------- Show Results --------
void showResults(struct Tree* root) {
    if (root == NULL) return;
    int i;
    printf("\nRegion: %s\n", root->region);
    for (i = 0; i < root->candidateCount; i++) {
        printf("%s - %d votes\n",
               root->candidates[i].name,
               root->candidates[i].votes);
    }

    showResults(root->left);
    showResults(root->right);
}

// -------- MAIN --------
int main() {
    int choice;
    char region[50];

    // Create tree
    struct Tree* root = createRegion("District1");
    root->left = createRegion("District2");
    root->right = createRegion("District3");

    while (1) {
        printf("\n--- Electoral Voting System ---\n");
        printf("1. Cast Vote\n");
        printf("2. Show Results\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter region (District1 / District2 / District3): ");
                scanf("%s", region);
                voteInRegion(root, region);
                break;

            case 2:
                showResults(root);
                break;

            case 3:
                exit(0);

            default:
                printf("? Invalid choice\n");
        }
    }
}