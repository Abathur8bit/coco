******************************************** QUICK DIRTY KEYREAD (continual)* BY INVISIBLE MAN*******************************************	org	$e00		GO	start	jsr	key	beq	startSTOP	sta	$400		;store the value	bra	start***************************************************************************************key	ldx	#keys		;lookup table for key vals	ldb	#%11111110	;bit value for column select (coco)	stb	$ff02		;store in column select	clrbcolstr	lda	$ff00test	cmpa	#%11110111	;row #4	beq	out		;store it	rol	$ff02		;next column in keyboard matrix (rol)	inc	$ff02	incb	cmpb	#8	bne	colstrout	lda	b,x	rtskeys	fcc	"X"	fcb	0		;skip Y 	fcc	"Z"	fcb	0,0		;up/down	fcb	9,8		;left/right	fcb	32		;space	fcb	0		;null value	end	GO