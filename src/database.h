#ifndef DATABASE_H
#define DATABASE_H

void createDatabase();
void addFileToSnapshot(const char* filename);
void registerSnapshot(const char* identifier, const char* text);
void getSnapshotInfo();
void getVersionContent(const char* identifier, int showContent);
void changeVersion(const char* identifier);
void restoreCurrentVersion();

#endif  // DATABASE_H
