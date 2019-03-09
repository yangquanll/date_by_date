ifeq ($(strip $(TRUSTKERNEL_TEE_SUPPORT)), yes)
	#copy ta
	PRODUCT_COPY_FILES += vendor/sprd/proprietories-source/trustkernel/source/ta/rpmbmac.elf:vendor/firmware/rpmbmac.elf
# TODO
endif
