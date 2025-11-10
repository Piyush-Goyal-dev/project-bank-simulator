#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Structure to represent a customer
typedef struct Customer {
    int arrival_time;       // When they arrived (in minutes)
    int service_start_time; // When they started being served
    int wait_time;          // How long they waited
} Customer;

// Structure for queue node (linked list)
typedef struct QueueNode {
    Customer customer;
    struct QueueNode *next;
} QueueNode;

// Structure to manage the queue
typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
    int size;
} Queue;

// Function declarations
void run_simulation(double lambda, int num_tellers);
int generate_arrivals(double lambda);
Queue* create_queue();
void enqueue(Queue *q, Customer customer);
Customer dequeue(Queue *q);
int is_empty(Queue *q);
void free_queue(Queue *q);
int has_busy_teller(int *teller_busy, int num_tellers);
void calculate_statistics(int *wait_times, int count);
int compare_ints(const void *a, const void *b);
double calculate_mean(int *data, int count);
double calculate_median(int *data, int count);
int calculate_mode(int *data, int count);
double calculate_std_dev(int *data, int count, double mean);
int find_max(int *data, int count);
int find_min(int *data, int count);

int main() {
    double lambda;
    int num_tellers = 1;
    
    // Seed random number generator
    srand(time(NULL));
    
    printf("=== Bank Queue Simulator ===\n");
    printf("Enter average customers per minute (lambda): ");
    scanf("%lf", &lambda);
    
    // Validate input
    if (lambda <= 0) {
        printf("Error: Lambda must be positive!\n");
        return 1;
    }
    
    printf("\nSimulating 8-hour day (480 minutes)...\n");
    run_simulation(lambda, num_tellers);
    
    return 0;
}

// Generate number of customer arrivals using Poisson distribution
int generate_arrivals(double lambda) {
    // Knuth's algorithm for generating Poisson-distributed random numbers
    double L = exp(-lambda);  // e^(-lambda)
    double p = 1.0;
    int k = 0;
    
    do {
        k++;
        p *= ((double)rand() / RAND_MAX);
    } while (p > L);
    
    return k - 1;
}

// Create an empty queue
Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

// Add customer to the queue (enqueue)
void enqueue(Queue *q, Customer customer) {
    QueueNode *new_node = (QueueNode*)malloc(sizeof(QueueNode));
    new_node->customer = customer;
    new_node->next = NULL;
    
    if (q->rear == NULL) {
        // Queue is empty
        q->front = new_node;
        q->rear = new_node;
    } else {
        // Add to the end
        q->rear->next = new_node;
        q->rear = new_node;
    }
    q->size++;
}

// Remove customer from the queue (dequeue)
Customer dequeue(Queue *q) {
    if (q->front == NULL) {
        Customer empty = {-1, -1, -1};
        return empty;
    }
    
    QueueNode *temp = q->front;
    Customer customer = temp->customer;
    
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    q->size--;
    return customer;
}

// Check if queue is empty
int is_empty(Queue *q) {
    return q->front == NULL;
}

// Free all memory used by queue
void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

// Check if any teller is busy
int has_busy_teller(int *teller_busy, int num_tellers) {
    for (int i = 0; i < num_tellers; i++) {
        if (teller_busy[i] > 0) {
            return 1;
        }
    }
    return 0;
}

// Comparison function for qsort
int compare_ints(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Calculate mean (average)
double calculate_mean(int *data, int count) {
    double sum = 0;
    for (int i = 0; i < count; i++) {
        sum += data[i];
    }
    return sum / count;
}

// Calculate median (middle value)
double calculate_median(int *data, int count) {
    // Sort the array first
    int *sorted = (int*)malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) {
        sorted[i] = data[i];
    }
    qsort(sorted, count, sizeof(int), compare_ints);
    
    double median;
    if (count % 2 == 0) {
        // Even number: average of two middle values
        median = (sorted[count/2 - 1] + sorted[count/2]) / 2.0;
    } else {
        // Odd number: middle value
        median = sorted[count/2];
    }
    
    free(sorted);
    return median;
}

// Calculate mode (most frequent value)
int calculate_mode(int *data, int count) {
    // Sort the array first
    int *sorted = (int*)malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) {
        sorted[i] = data[i];
    }
    qsort(sorted, count, sizeof(int), compare_ints);
    
    int mode = sorted[0];
    int max_frequency = 1;
    int current_frequency = 1;
    
    for (int i = 1; i < count; i++) {
        if (sorted[i] == sorted[i-1]) {
            current_frequency++;
        } else {
            if (current_frequency > max_frequency) {
                max_frequency = current_frequency;
                mode = sorted[i-1];
            }
            current_frequency = 1;
        }
    }
    
    // Check last sequence
    if (current_frequency > max_frequency) {
        mode = sorted[count-1];
    }
    
    free(sorted);
    return mode;
}

// Calculate standard deviation
double calculate_std_dev(int *data, int count, double mean) {
    double sum_squared_diff = 0;
    
    for (int i = 0; i < count; i++) {
        double diff = data[i] - mean;
        sum_squared_diff += diff * diff;
    }
    
    double variance = sum_squared_diff / count;
    return sqrt(variance);
}

// Find maximum value
int find_max(int *data, int count) {
    int max = data[0];
    for (int i = 1; i < count; i++) {
        if (data[i] > max) {
            max = data[i];
        }
    }
    return max;
}

// Find minimum value
int find_min(int *data, int count) {
    int min = data[0];
    for (int i = 1; i < count; i++) {
        if (data[i] < min) {
            min = data[i];
        }
    }
    return min;
}

// Complete statistics calculation and report
void calculate_statistics(int *wait_times, int count) {
    printf("\n========================================\n");
    printf("     WAIT TIME ANALYSIS REPORT\n");
    printf("========================================\n\n");
    
    // Calculate all statistics
    double mean = calculate_mean(wait_times, count);
    double median = calculate_median(wait_times, count);
    int mode = calculate_mode(wait_times, count);
    double std_dev = calculate_std_dev(wait_times, count, mean);
    int max_wait = find_max(wait_times, count);
    int min_wait = find_min(wait_times, count);
    
    // Display results
    printf("📊 Central Tendency Measures:\n");
    printf("   Mean Wait Time:     %.2f minutes\n", mean);
    printf("   Median Wait Time:   %.2f minutes\n", median);
    printf("   Mode Wait Time:     %d minutes\n\n", mode);
    
    printf("📈 Dispersion Measures:\n");
    printf("   Standard Deviation: %.2f minutes\n", std_dev);
    printf("   Variance:           %.2f minutes²\n\n", std_dev * std_dev);
    
    printf("⏱️  Extreme Values:\n");
    printf("   Minimum Wait Time:  %d minutes\n", min_wait);
    printf("   Maximum Wait Time:  %d minutes\n\n", max_wait);
    
    printf("========================================\n");
    
    // Provide recommendations
    printf("\n💡 RECOMMENDATIONS:\n");
    if (mean > 10) {
        printf("⚠️  Average wait time exceeds 10 minutes!\n");
        printf("   Consider hiring additional tellers.\n");
    } else if (mean > 5) {
        printf("⚡ Wait times are moderate.\n");
        printf("   Monitor during peak hours.\n");
    } else {
        printf("✅ Wait times are excellent!\n");
        printf("   Current staffing is adequate.\n");
    }
    
    if (max_wait > 30) {
        printf("⚠️  Some customers waited over 30 minutes!\n");
        printf("   This may lead to customer dissatisfaction.\n");
    }
    
    printf("========================================\n");
}

// Main simulation function
void run_simulation(double lambda, int num_tellers) {
    Queue *customer_queue = create_queue();
    
    // Dynamic array to store all wait times
    int *wait_times = NULL;
    int wait_times_count = 0;
    int wait_times_capacity = 100;
    wait_times = (int*)malloc(wait_times_capacity * sizeof(int));
    
    // Track teller availability (0 = free, >0 = busy for X more minutes)
    int *teller_busy = (int*)calloc(num_tellers, sizeof(int));
    
    int total_customers_arrived = 0;
    int total_customers_served = 0;
    int max_queue_size = 0;
    
    // Main simulation loop: 480 minutes (8 hours)
    for (int minute = 0; minute < 480; minute++) {
        
        // STEP 1: Generate new customer arrivals for this minute
        int arrivals = generate_arrivals(lambda);
        
        for (int i = 0; i < arrivals; i++) {
            Customer new_customer;
            new_customer.arrival_time = minute;
            new_customer.service_start_time = -1;  // Not served yet
            new_customer.wait_time = 0;
            
            enqueue(customer_queue, new_customer);
            total_customers_arrived++;
        }
        
        // Track maximum queue size
        if (customer_queue->size > max_queue_size) {
            max_queue_size = customer_queue->size;
        }
        
        // STEP 2: Update teller status (decrement busy time)
        for (int t = 0; t < num_tellers; t++) {
            if (teller_busy[t] > 0) {
                teller_busy[t]--;
            }
        }
        
        // STEP 3: Assign waiting customers to free tellers
        for (int t = 0; t < num_tellers; t++) {
            if (teller_busy[t] == 0 && !is_empty(customer_queue)) {
                // Teller is free and there's a customer waiting
                Customer customer = dequeue(customer_queue);
                
                customer.service_start_time = minute;
                customer.wait_time = minute - customer.arrival_time;
                
                // Store wait time in our dynamic array
                if (wait_times_count >= wait_times_capacity) {
                    // Resize array if needed
                    wait_times_capacity *= 2;
                    wait_times = (int*)realloc(wait_times, wait_times_capacity * sizeof(int));
                }
                wait_times[wait_times_count++] = customer.wait_time;
                
                // Teller serves this customer (takes 2-4 minutes randomly)
                int service_time = 2 + (rand() % 3);  // 2, 3, or 4 minutes
                teller_busy[t] = service_time;
                
                total_customers_served++;
            }
        }
    }
    
    // STEP 4: After 480 minutes, finish serving remaining customers
    printf("\nSimulation complete! Processing remaining customers...\n");
    
    int extra_time = 0;
    while (!is_empty(customer_queue) || has_busy_teller(teller_busy, num_tellers)) {
        extra_time++;
        
        // Update tellers
        for (int t = 0; t < num_tellers; t++) {
            if (teller_busy[t] > 0) {
                teller_busy[t]--;
            }
        }
        
        // Serve remaining customers
        for (int t = 0; t < num_tellers; t++) {
            if (teller_busy[t] == 0 && !is_empty(customer_queue)) {
                Customer customer = dequeue(customer_queue);
                customer.service_start_time = 480 + extra_time;
                customer.wait_time = (480 + extra_time) - customer.arrival_time;
                
                if (wait_times_count >= wait_times_capacity) {
                    wait_times_capacity *= 2;
                    wait_times = (int*)realloc(wait_times, wait_times_capacity * sizeof(int));
                }
                wait_times[wait_times_count++] = customer.wait_time;
                
                int service_time = 2 + (rand() % 3);
                teller_busy[t] = service_time;
                total_customers_served++;
            }
        }
    }
    
    // STEP 5: Print simulation statistics
    printf("\n========== SIMULATION RESULTS ==========\n");
    printf("Total customers arrived: %d\n", total_customers_arrived);
    printf("Total customers served: %d\n", total_customers_served);
    printf("Maximum queue size: %d\n", max_queue_size);
    printf("Extra time needed: %d minutes\n", extra_time);
    
    // Calculate and print statistics
    if (wait_times_count > 0) {
        calculate_statistics(wait_times, wait_times_count);
    }
    
    // Clean up memory
    free(wait_times);
    free(teller_busy);
    free_queue(customer_queue);
}