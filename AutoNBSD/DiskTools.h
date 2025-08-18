#include <stdlib.h> 
#include <string.h>

#define MAX_DISKS 32
#define MAX_DISKS_NAME_LEN 16 

typedef struct {
    char name[MAX_DISKS_NAME_LEN];     // Device name (e.g., "wd0")
    char type[32];                    // Disk type
    char label[32];                   // Disk label
    unsigned int sector_size;         // Sector size in bytes
    unsigned int cylinders;           // Number of cylinders
    unsigned int heads;               // Number of heads
    unsigned int sectors_per_track;   // Sectors per track
    unsigned long long size;          // Total size in bytes
    int valid;                        // Flag indicating if this entry is valid
} disk_info_t;

int get_disk_info(const char *dev_name, disk_info_t *disk);
disk_info_t* get_disk_by_index(int index);
disk_info_t* get_disk_by_name(const char *name); 
int get_disk_count();
int scan_disks();



