
#ifndef NDS_LOADER_ARM9_H
#define NDS_LOADER_ARM9_H


#ifdef __cplusplus
extern "C" {
#endif

#define LOAD_DEFAULT_NDS 0

	bool runNds(const void* loader, u32 loaderSize, u32 cluster, bool initDisc, bool dldiPatchNds);

	bool runNdsFile(const char* filename);

#ifdef __cplusplus
}
#endif

#endif // NDS_LOADER_ARM7_H
