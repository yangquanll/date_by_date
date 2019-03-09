#ifndef VRPMB_H_
#define VRPMB_H_

#ifdef _cpluscplus
extern "C" {
#endif

int vrpmb_init(const char *vrpmb_root_path);
void vrpmb_release(void);

int vrpmb_send(struct storage_msg *msg, const void *r, size_t req_len);

#ifdef _cpluscplus
}
#endif

#endif
