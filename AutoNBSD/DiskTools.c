#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/disklabel.h>
#include <sys/ioctl.h>
#include <sys/dkio.h>
#include <sys/disk.h> 

#include "DiskTools.h"
/*
// Structure to hold disk information
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
*/

static disk_info_t disks[MAX_DISKS];
static int disk_count = 0;


static int is_disk_device(const char *dev_name) {
    char path[64];
    int fd;
    struct disklabel dl;
    
    snprintf(path, sizeof(path), "/dev/%s", dev_name);
    fd = open(path, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        return 0;
    }
    
  
    if (ioctl(fd, DIOCGDINFO, &dl) == 0) {
        close(fd);
        return 1;
    }
    
    close(fd);
    return 0;
}

int get_disk_info(const char *dev_name, disk_info_t *disk) {
    char path[64];
    int fd;
    struct disklabel dl;
    struct disk_geom geom;
    
    snprintf(path, sizeof(path), "/dev/%s", dev_name);
    fd = open(path, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        return -1;
    }
    

    if (ioctl(fd, DIOCGDINFO, &dl) == -1) {
        close(fd);
        return -1;
    }
    
/*
    if (ioctl(fd, DIOCGEOM, &geom) == -1) {
        close(fd);
        return -1;
    }
*/ 

    strncpy(disk->name, dev_name, MAX_DISKS_NAME_LEN - 1);
    disk->name[MAX_DISKS_NAME_LEN - 1] = '\0';
    
    strncpy(disk->type, dl.d_typename, sizeof(disk->type) - 1);
    disk->type[sizeof(disk->type) - 1] = '\0';
    
    strncpy(disk->label, dl.d_packname, sizeof(disk->label) - 1);
    disk->label[sizeof(disk->label) - 1] = '\0';
    
    disk->sector_size = dl.d_secsize;
    disk->cylinders = dl.d_ncylinders;
    disk->heads = dl.d_nheads;
    disk->sectors_per_track = dl.d_nsectors;
    
    
    disk->size = (unsigned long long)dl.d_secperunit * dl.d_secsize;
    
    disk->valid = 1;
    
    close(fd);
    return 0;
}


int scan_disks() {
    const char *prefixes[] = {"wd", "sd", "cd", NULL}; 
    int i, unit;
    
    
    disk_count = 0;
    
    
    for (i = 0; i < MAX_DISKS; i++) {
        disks[i].valid = 0;
    }
    
  
    for (i = 0; prefixes[i] != NULL && disk_count < MAX_DISKS; i++) {
        for (unit = 0; unit < 16 && disk_count < MAX_DISKS; unit++) {
            char dev_name[16];
            snprintf(dev_name, sizeof(dev_name), "%s%d", prefixes[i], unit);
            
            if (is_disk_device(dev_name)) {
                if (get_disk_info(dev_name, &disks[disk_count]) == 0) {
                    disk_count++;
                }
            }
        }
    }
    
    return disk_count;
}


int get_disk_count() {
    return disk_count;
}


disk_info_t* get_disk_by_index(int index) {
    if (index < 0 || index >= disk_count) {
        return NULL;
    }
    return &disks[index];
}


disk_info_t* get_disk_by_name(const char *name) {
    int i;
    for (i = 0; i < disk_count; i++) {
        if (strcmp(disks[i].name, name) == 0) {
            return &disks[i];
        }
    }
    return NULL;
}


void print_disk_info(disk_info_t *disk) {
    if (!disk || !disk->valid) {
        printf("Invalid disk info\n");
        return;
    }
    
    printf("Disk: %s\n", disk->name);
    printf("  Type: %s\n", disk->type);
    printf("  Label: %s\n", disk->label);
    printf("  Sector size: %u bytes\n", disk->sector_size);
    printf("  Cylinders: %u\n", disk->cylinders);
    printf("  Heads: %u\n", disk->heads);
    printf("  Sectors/track: %u\n", disk->sectors_per_track);
    printf("  Size: %llu bytes (%.2f GB)\n", 
           disk->size, 
           (double)disk->size / (1024*1024*1024));
}


void print_all_disks() {
    int i;
    printf("Found %d disk(s):\n", disk_count);
    for (i = 0; i < disk_count; i++) {
        printf("\n--- Disk %d ---\n", i);
        print_disk_info(&disks[i]);
    }
}


int cli_usage() {
    printf("NetBSD Disk Detection Utility\n");
    printf("=============================\n");
    

    if (scan_disks() < 0) {
        printf("Error scanning for disks\n");
        return 1;
    }
    
    
    print_all_disks();
    
    
    if (get_disk_count() > 0) {
        disk_info_t *disk = get_disk_by_index(0);
        if (disk) {
            printf("\nFirst disk information:\n");
            print_disk_info(disk);
        }
    }
    
    return 0;
}
