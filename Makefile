.PHONY: clean All

All:
	@echo "----------Building project:[ RatVideo - Debug ]----------"
	@"$(MAKE)" -f  "RatVideo.mk"
clean:
	@echo "----------Cleaning project:[ RatVideo - Debug ]----------"
	@"$(MAKE)" -f  "RatVideo.mk" clean
