#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void transmission(int totalFrames, int windowSize) {
    int ack[totalFrames + 1]; // To keep track of acknowledgments
    int framesSent = 0, retransmissions = 0;

    // Initialize all frames as unacknowledged (0)
    for (int i = 1; i <= totalFrames; i++) {
        ack[i] = 0;
    }

    srand(time(NULL)); // Seed for random number generation

    while (framesSent < totalFrames) {
        // Send frames within the window size
        printf("Sending frames in the window:\n");
        for (int i = 0; i < windowSize && (framesSent + i + 1) <= totalFrames; i++) {
            int frame = framesSent + i + 1;
            if (ack[frame] == 0) { // If frame is not yet acknowledged
                printf("  Sending Frame %d...\n", frame);
            }
        }

        printf("Receiving Acknowledgments:\n");
        for (int i = 0; i < windowSize && (framesSent + i + 1) <= totalFrames; i++) {
            int frame = framesSent + i + 1;
            if (ack[frame] == 0) { // If frame has not been acknowledged
                int received = rand() % 2; // Randomly decide if ACK is received (0 = ACK received, 1 = Timeout)
                if (received == 0) {
                    printf("  Acknowledgment received for Frame %d\n", frame);
                    ack[frame] = 1; // Mark frame as acknowledged
                } else {
                    printf("  Timeout! Frame %d not received. Retransmitting...\n", frame);
                    retransmissions++;
                }
            }
        }

        // Move the window forward only for frames that have been acknowledged
        while (framesSent < totalFrames && ack[framesSent + 1] == 1) {
            framesSent++;
        }

        printf("\n");
    }

    printf("All frames have been sent and acknowledged.\n");
    printf("Total retransmissions: %d\n", retransmissions);
}

int main() {
    int totalFrames, windowSize;

    printf("Enter the total number of frames: ");
    scanf("%d", &totalFrames);
    printf("Enter the window size: ");
    scanf("%d", &windowSize);

    transmission(totalFrames, windowSize);

    return 0;
}
