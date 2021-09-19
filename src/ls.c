#include "../global.h"

int max(int a, int b)
{
    return (a > b) ? a : b;
}
char *replace_string(char *str, char *old, char *new)
{
    int old_len = strlen(old);
    int new_len = strlen(new);
    int cnt = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (strncmp(&str[i], old, old_len) == 0)
        {
            cnt++;
            i = i + old_len - 1;
        }
    }
    int res_len = strlen(str) + cnt * (new_len - old_len) + 1;
    char *result = (char *)calloc(res_len, sizeof(char));
    assert(result);

    int j = 0;
    for (int i = 0; str[i] != '\0'; i++, j++)
    {
        if (strncmp(&str[i], old, old_len) == 0)
        {
            strcpy(&result[j], new);
            i = i + old_len - 1;
            j = j + new_len - 1;
        }
        else
        {
            result[j] = str[i];
        }
    }
    return result;
}
void ls_dir(char *path, bool a, bool l)
{
    char *resolved_path = replace_string(path, "~", home_folder);
    char *real_path = realpath(resolved_path, NULL);
    DIR *dir = opendir(real_path);
    int field_username = 0, field_groupname = 0, field_size = 0, field_nlink = 0, field_time = 0;
    if (dir == NULL)
    {
        perror("Directory");
        return;
    }
    char time_buf[1000];
    long long int total = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)))
    {
        if (!a && entry->d_name[0] == '.')
        {
            continue;
        }
        char file_name[1000];
        sprintf(file_name, "%s/%s", real_path, entry->d_name);
        struct stat stat;
        lstat(file_name, &stat);
        field_username = max(field_username, strlen(getpwuid(stat.st_uid)->pw_name));
        field_groupname = max(field_groupname, strlen(getgrgid(stat.st_uid)->gr_name));
        field_nlink = max(field_nlink, (int)log10((int)stat.st_nlink) + 1);
        field_size = max(field_size, (int)log10((int)stat.st_size) + 1);
        struct tm *file_t = NULL;
        file_t = localtime(&(stat.st_ctime));
        time_t curr_t;
        time(&curr_t);
        long long int months_6 = 180 * 24 * 60 * 60;
        long long int file_t_seconds = (file_t->tm_sec) + file_t->tm_min * 60 + file_t->tm_hour * 60 * 60 + (file_t->tm_mday - 1) * 24 * 60 * 60 + file_t->tm_yday * 24 * 60 * 60 + (file_t->tm_year - 70) * 365 * 24 * 60 * 60;
        if (curr_t - file_t_seconds < months_6)
        {
            strftime(time_buf, 26, "%b %d %H:%M", file_t);
        }
        else
        {
            strftime(time_buf, 26, "%b %d %Y", file_t);
        }
        field_time = max(field_time, strlen(time_buf));
        total += stat.st_blocks / 2;
    }
    char perms[11];
    closedir(dir);
    dir = opendir(real_path);
    if (l)
        printf("total %lld\n", total);
    while ((entry = readdir(dir)))
    {
        if (!a && entry->d_name[0] == '.')
        {
            continue;
        }
        char file_name[1000];
        sprintf(file_name, "%s/%s", real_path, entry->d_name);
        struct stat stat;
        lstat(file_name, &stat);
        if (l)
        {
            perms[0] = (S_ISDIR(stat.st_mode)) ? 'd' : '-';
            perms[1] = (stat.st_mode & S_IRUSR) ? 'r' : '-';
            perms[2] = (stat.st_mode & S_IWUSR) ? 'w' : '-';
            perms[3] = (stat.st_mode & S_IXUSR) ? 'x' : '-';
            perms[4] = (stat.st_mode & S_IRGRP) ? 'r' : '-';
            perms[5] = (stat.st_mode & S_IWGRP) ? 'w' : '-';
            perms[6] = (stat.st_mode & S_IXGRP) ? 'x' : '-';
            perms[7] = (stat.st_mode & S_IROTH) ? 'r' : '-';
            perms[8] = (stat.st_mode & S_IWOTH) ? 'w' : '-';
            perms[9] = (stat.st_mode & S_IXOTH) ? 'x' : '-';
            perms[10] = '\0';
            printf("%s ", perms);
            printf("%*ld ", field_nlink, stat.st_nlink);
            printf("%*s ", field_username, getpwuid(stat.st_uid)->pw_name);
            printf("%*s ", field_groupname, getgrgid(stat.st_uid)->gr_name);
            printf("%*ld ", field_size, stat.st_size);
            struct tm *file_t = NULL;
            file_t = localtime(&(stat.st_ctime));
            time_t curr_t;
            time(&curr_t);
            long long int months_6 = 180 * 24 * 60 * 60;
            long long int file_t_seconds = (file_t->tm_sec) + file_t->tm_min * 60 + file_t->tm_hour * 60 * 60 + (file_t->tm_mday - 1) * 24 * 60 * 60 + file_t->tm_yday * 24 * 60 * 60 + (file_t->tm_year - 70) * 365 * 24 * 60 * 60;
            if (curr_t - file_t_seconds < months_6)
            {
                strftime(time_buf, 26, "%b %d %H:%M", file_t);
                printf("%*s ", field_time, time_buf);
            }
            else
            {
                strftime(time_buf, 26, "%b %d %Y", file_t);
                printf("%*s ", field_time, time_buf);
            }
        }
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
    free(real_path);
    free(resolved_path);
}
void ls_file(char *path, bool a, bool l)
{
    char *resolved_path = replace_string(path, "~", home_folder);
    char *real_path = realpath(resolved_path, NULL);
    struct stat stat;
    lstat(real_path, &stat);
    char perms[11];
    char time_buf[1000];
    if (l)
    {
        perms[0] = (S_ISDIR(stat.st_mode)) ? 'd' : '-';
        perms[1] = (stat.st_mode & S_IRUSR) ? 'r' : '-';
        perms[2] = (stat.st_mode & S_IWUSR) ? 'w' : '-';
        perms[3] = (stat.st_mode & S_IXUSR) ? 'x' : '-';
        perms[4] = (stat.st_mode & S_IRGRP) ? 'r' : '-';
        perms[5] = (stat.st_mode & S_IWGRP) ? 'w' : '-';
        perms[6] = (stat.st_mode & S_IXGRP) ? 'x' : '-';
        perms[7] = (stat.st_mode & S_IROTH) ? 'r' : '-';
        perms[8] = (stat.st_mode & S_IWOTH) ? 'w' : '-';
        perms[9] = (stat.st_mode & S_IXOTH) ? 'x' : '-';
        perms[10] = '\0';
        printf("%s ", perms);
        printf("%ld ", stat.st_nlink);
        printf("%s ", getpwuid(stat.st_uid)->pw_name);
        printf("%s ", getgrgid(stat.st_uid)->gr_name);
        printf("%ld ", stat.st_size);
        struct tm *file_t = NULL;
        file_t = localtime(&(stat.st_ctime));
        time_t curr_t;
        time(&curr_t);
        long long int months_6 = 180 * 24 * 60 * 60;
        long long int file_t_seconds = (file_t->tm_sec) + file_t->tm_min * 60 + file_t->tm_hour * 60 * 60 + (file_t->tm_mday - 1) * 24 * 60 * 60 + file_t->tm_yday * 24 * 60 * 60 + (file_t->tm_year - 70) * 365 * 24 * 60 * 60;
        if (curr_t - file_t_seconds < months_6)
        {
            strftime(time_buf, 26, "%b %d %H:%M", file_t);
            printf("%s ", time_buf);
        }
        else
        {
            strftime(time_buf, 26, "%b %d %Y", file_t);
            printf("%s ", time_buf);
        }
    }
    printf("%s\n", resolved_path);
    free(resolved_path);
    free(real_path);
}
void ls(char *path, bool a, bool l)
{
    bool empty = 0;
    if (path == NULL)
    {
        empty = 1;
        path = (char *)calloc(2, sizeof(char));
        strcpy(path, ".");
    }
    char *resolved_path = replace_string(path, "~", home_folder);
    char *real_path = realpath(resolved_path, NULL);
    struct stat stat;
    lstat(real_path, &stat);
    if (S_ISDIR(stat.st_mode))
    {
        ls_dir(real_path, a, l);
    }
    else
    {
        ls_file(resolved_path, a, l);
    }
    if (empty)
    {
        free(path);
    }
    free(resolved_path);
    free(real_path);
}